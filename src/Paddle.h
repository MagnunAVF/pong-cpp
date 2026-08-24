#pragma once

#include <SDL.h>

class Paddle {
public:
    Paddle(float x, float y, float width, float height, float speed);

    void SetVelocityY(float velocity_y);
    void SetY(float y);
    void Update(float dt, int window_height);
    void Render(SDL_Renderer* renderer) const;

    const SDL_FRect& rect() const { return rect_; }
    float speed() const { return speed_; }

private:
    SDL_FRect rect_;
    float velocity_y_ = 0.0f;
    float speed_;
};
