#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "Ball.h"
#include "Paddle.h"

enum class GameState {
    Playing,
    GameOver,
};

class Game {
public:
    bool Init();
    void Run();
    void Shutdown();

private:
    void ProcessInput();
    void Update(float dt);
    void Render();

    void ResetBall();
    void RestartGame();

    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    Paddle* left_paddle_ = nullptr;
    Paddle* right_paddle_ = nullptr;
    Ball* ball_ = nullptr;
    TTF_Font* font_ = nullptr;
    SDL_Texture* left_hint_texture_ = nullptr;
    SDL_Texture* right_hint_texture_ = nullptr;
    SDL_Texture* game_over_texture_ = nullptr;
    int left_score_ = 0;
    int right_score_ = 0;
    GameState state_ = GameState::Playing;
    bool running_ = false;
};
