#pragma once

#include <SDL.h>

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
    int left_score_ = 0;
    int right_score_ = 0;
    GameState state_ = GameState::Playing;
    bool running_ = false;
};
