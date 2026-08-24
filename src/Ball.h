#pragma once

#include <SDL.h>

class Ball {
public:
    Ball(float x, float y, float size, float velocity_x, float velocity_y);

    void Update(float dt, int window_height);
    void Render(SDL_Renderer* renderer) const;

    void SetVelocity(float velocity_x, float velocity_y);

    const SDL_FRect& rect() const { return rect_; }
    float velocity_x() const { return velocity_x_; }
    float velocity_y() const { return velocity_y_; }

private:
    SDL_FRect rect_;
    float velocity_x_;
    float velocity_y_;
};
