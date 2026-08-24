#include "Game.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>

namespace {
constexpr int kWindowWidth = 800;
constexpr int kWindowHeight = 600;
constexpr float kFixedTimestep = 1.0f / 60.0f;

constexpr float kPaddleWidth = 12.0f;
constexpr float kPaddleHeight = 80.0f;
constexpr float kPaddleMargin = 30.0f;
constexpr float kPaddleSpeed = 300.0f;

constexpr float kBallSize = 12.0f;
constexpr float kBallSpeed = 250.0f;

constexpr int kWinningScore = 5;

constexpr float kDigitWidth = 20.0f;
constexpr float kDigitHeight = 30.0f;
constexpr float kDigitThickness = 4.0f;
constexpr float kDigitSpacing = 10.0f;
constexpr float kScoreMarginTop = 20.0f;
constexpr float kScoreMarginCenter = 40.0f;

constexpr float kDashWidth = 4.0f;
constexpr float kDashHeight = 16.0f;
constexpr float kDashGap = 12.0f;

constexpr const char* kFontPath = "/System/Library/Fonts/Monaco.ttf";
constexpr int kFontSize = 16;
constexpr float kHintMarginBottom = 40.0f;

// Segment order: a (top), b (top-right), c (bottom-right), d (bottom),
// e (bottom-left), f (top-left), g (middle).
constexpr bool kDigitSegments[10][7] = {
    {true, true, true, true, true, true, false},      // 0
    {false, true, true, false, false, false, false},  // 1
    {true, true, false, true, true, false, true},      // 2
    {true, true, true, true, false, false, true},      // 3
    {false, true, true, false, false, true, true},     // 4
    {true, false, true, true, false, true, true},      // 5
    {true, false, true, true, true, true, true},       // 6
    {true, true, true, false, false, false, false},    // 7
    {true, true, true, true, true, true, true},        // 8
    {true, true, true, true, false, true, true},       // 9
};

void DrawDigit(SDL_Renderer* renderer, int digit, float x, float y) {
    if (digit < 0 || digit > 9) {
        return;
    }

    const bool* segments = kDigitSegments[digit];
    const float half_h = kDigitHeight / 2.0f;

    const SDL_FRect rects[7] = {
        {x, y, kDigitWidth, kDigitThickness},
        {x + kDigitWidth - kDigitThickness, y, kDigitThickness, half_h},
        {x + kDigitWidth - kDigitThickness, y + half_h, kDigitThickness, half_h},
        {x, y + kDigitHeight - kDigitThickness, kDigitWidth, kDigitThickness},
        {x, y + half_h, kDigitThickness, half_h},
        {x, y, kDigitThickness, half_h},
        {x, y + half_h - kDigitThickness / 2.0f, kDigitWidth, kDigitThickness},
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < 7; ++i) {
        if (segments[i]) {
            SDL_RenderFillRectF(renderer, &rects[i]);
        }
    }
}

void DrawNumber(SDL_Renderer* renderer, int number, float x, float y) {
    char buffer[12];
    std::snprintf(buffer, sizeof(buffer), "%d", number);

    float cursor_x = x;
    for (const char* c = buffer; *c != '\0'; ++c) {
        DrawDigit(renderer, *c - '0', cursor_x, y);
        cursor_x += kDigitWidth + kDigitSpacing;
    }
}

void DrawCenterLine(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    const float x = (kWindowWidth - kDashWidth) / 2.0f;
    for (float y = 0.0f; y < kWindowHeight; y += kDashHeight + kDashGap) {
        const SDL_FRect dash{x, y, kDashWidth, kDashHeight};
        SDL_RenderFillRectF(renderer, &dash);
    }
}

SDL_Texture* CreateTextTexture(SDL_Renderer* renderer, TTF_Font* font,
                                const char* text) {
    const SDL_Color white{255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, white);
    if (surface == nullptr) {
        std::fprintf(stderr, "TTF_RenderText_Blended failed: %s\n",
                     TTF_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void DrawTextureCentered(SDL_Renderer* renderer, SDL_Texture* texture,
                          float center_x, float y) {
    if (texture == nullptr) {
        return;
    }

    int width = 0;
    int height = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    const SDL_FRect dst{center_x - width / 2.0f, y,
                         static_cast<float>(width), static_cast<float>(height)};
    SDL_RenderCopyF(renderer, texture, nullptr, &dst);
}
}  // namespace

bool Game::Init() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }

    window_ = SDL_CreateWindow(
        "Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        kWindowWidth, kWindowHeight, SDL_WINDOW_SHOWN);
    if (window_ == nullptr) {
        std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    renderer_ = SDL_CreateRenderer(
        window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer_ == nullptr) {
        std::fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window_);
        SDL_Quit();
        return false;
    }

    const float paddle_y = (kWindowHeight - kPaddleHeight) / 2.0f;
    left_paddle_ = new Paddle(kPaddleMargin, paddle_y, kPaddleWidth,
                               kPaddleHeight, kPaddleSpeed);
    right_paddle_ = new Paddle(kWindowWidth - kPaddleMargin - kPaddleWidth,
                                paddle_y, kPaddleWidth, kPaddleHeight,
                                kPaddleSpeed);

    ball_ = new Ball((kWindowWidth - kBallSize) / 2.0f,
                      (kWindowHeight - kBallSize) / 2.0f, kBallSize,
                      kBallSpeed, kBallSpeed * 0.6f);

    if (TTF_Init() != 0) {
        std::fprintf(stderr, "TTF_Init failed: %s\n", TTF_GetError());
    } else {
        font_ = TTF_OpenFont(kFontPath, kFontSize);
        if (font_ == nullptr) {
            std::fprintf(stderr, "TTF_OpenFont failed: %s\n", TTF_GetError());
        } else {
            left_hint_texture_ =
                CreateTextTexture(renderer_, font_, "W/S: MOVE");
            right_hint_texture_ =
                CreateTextTexture(renderer_, font_, "UP/DOWN: MOVE");
            game_over_texture_ = CreateTextTexture(
                renderer_, font_, "GAME OVER - PRESS ENTER TO RESTART");
        }
    }

    running_ = true;
    return true;
}

void Game::Run() {
    Uint64 previous_ticks = SDL_GetPerformanceCounter();
    float accumulator = 0.0f;

    while (running_) {
        const Uint64 current_ticks = SDL_GetPerformanceCounter();
        const float frame_time =
            static_cast<float>(current_ticks - previous_ticks) /
            static_cast<float>(SDL_GetPerformanceFrequency());
        previous_ticks = current_ticks;

        accumulator += frame_time;

        ProcessInput();

        while (accumulator >= kFixedTimestep) {
            Update(kFixedTimestep);
            accumulator -= kFixedTimestep;
        }

        Render();
    }
}

void Game::Shutdown() {
    delete left_paddle_;
    delete right_paddle_;
    delete ball_;

    SDL_DestroyTexture(left_hint_texture_);
    SDL_DestroyTexture(right_hint_texture_);
    SDL_DestroyTexture(game_over_texture_);
    if (font_ != nullptr) {
        TTF_CloseFont(font_);
    }
    TTF_Quit();

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running_ = false;
        } else if (event.type == SDL_KEYDOWN &&
                   event.key.keysym.scancode == SDL_SCANCODE_RETURN &&
                   state_ == GameState::GameOver) {
            RestartGame();
        }
    }

    if (state_ != GameState::Playing) {
        return;
    }

    const Uint8* keys = SDL_GetKeyboardState(nullptr);

    float left_velocity = 0.0f;
    if (keys[SDL_SCANCODE_W]) {
        left_velocity -= left_paddle_->speed();
    }
    if (keys[SDL_SCANCODE_S]) {
        left_velocity += left_paddle_->speed();
    }
    left_paddle_->SetVelocityY(left_velocity);

    float right_velocity = 0.0f;
    if (keys[SDL_SCANCODE_UP]) {
        right_velocity -= right_paddle_->speed();
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        right_velocity += right_paddle_->speed();
    }
    right_paddle_->SetVelocityY(right_velocity);
}

void Game::Update(float dt) {
    if (state_ != GameState::Playing) {
        return;
    }

    left_paddle_->Update(dt, kWindowHeight);
    right_paddle_->Update(dt, kWindowHeight);
    ball_->Update(dt, kWindowHeight);

    if (ball_->velocity_x() < 0.0f &&
        SDL_HasIntersectionF(&ball_->rect(), &left_paddle_->rect())) {
        ball_->BounceOffPaddle(left_paddle_->rect());
    } else if (ball_->velocity_x() > 0.0f &&
               SDL_HasIntersectionF(&ball_->rect(), &right_paddle_->rect())) {
        ball_->BounceOffPaddle(right_paddle_->rect());
    }

    if (ball_->rect().x + ball_->rect().w < 0.0f) {
        ++right_score_;
        std::printf("Score: %d - %d\n", left_score_, right_score_);
        ResetBall();
    } else if (ball_->rect().x > static_cast<float>(kWindowWidth)) {
        ++left_score_;
        std::printf("Score: %d - %d\n", left_score_, right_score_);
        ResetBall();
    }

    if (left_score_ >= kWinningScore || right_score_ >= kWinningScore) {
        state_ = GameState::GameOver;
        std::printf("Game over! Press Enter to restart.\n");
    }
}

void Game::ResetBall() {
    ball_->Reset((kWindowWidth - kBallSize) / 2.0f,
                 (kWindowHeight - kBallSize) / 2.0f, kBallSpeed);
}

void Game::RestartGame() {
    left_score_ = 0;
    right_score_ = 0;

    const float paddle_y = (kWindowHeight - kPaddleHeight) / 2.0f;
    left_paddle_->SetY(paddle_y);
    right_paddle_->SetY(paddle_y);

    ResetBall();
    state_ = GameState::Playing;
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    DrawCenterLine(renderer_);

    left_paddle_->Render(renderer_);
    right_paddle_->Render(renderer_);
    ball_->Render(renderer_);

    DrawNumber(renderer_, left_score_,
               kWindowWidth / 2.0f - kScoreMarginCenter - kDigitWidth,
               kScoreMarginTop);
    DrawNumber(renderer_, right_score_,
               kWindowWidth / 2.0f + kScoreMarginCenter, kScoreMarginTop);

    DrawTextureCentered(renderer_, left_hint_texture_, kWindowWidth / 4.0f,
                         kWindowHeight - kHintMarginBottom);
    DrawTextureCentered(renderer_, right_hint_texture_,
                         3.0f * kWindowWidth / 4.0f,
                         kWindowHeight - kHintMarginBottom);

    if (state_ == GameState::GameOver) {
        DrawTextureCentered(renderer_, game_over_texture_,
                             kWindowWidth / 2.0f, kWindowHeight / 2.0f);
    }

    SDL_RenderPresent(renderer_);
}
