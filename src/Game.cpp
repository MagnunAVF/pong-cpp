#include "Game.h"

#include <cstdio>

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
}  // namespace

bool Game::Init() {
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
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running_ = false;
        }
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
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    left_paddle_->Render(renderer_);
    right_paddle_->Render(renderer_);
    ball_->Render(renderer_);

    SDL_RenderPresent(renderer_);
}
