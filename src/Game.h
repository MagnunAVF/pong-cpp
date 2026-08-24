#pragma once

#include <SDL.h>

#include "Ball.h"
#include "Paddle.h"

class Game {
public:
    bool Init();
    void Run();
    void Shutdown();

private:
    void ProcessInput();
    void Update(float dt);
    void Render();

    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    Paddle* left_paddle_ = nullptr;
    Paddle* right_paddle_ = nullptr;
    Ball* ball_ = nullptr;
    bool running_ = false;
};
