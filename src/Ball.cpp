#include "Ball.h"

Ball::Ball(float x, float y, float size, float velocity_x, float velocity_y)
    : rect_{x, y, size, size}, velocity_x_(velocity_x), velocity_y_(velocity_y) {}

void Ball::Update(float dt, int window_height) {
    rect_.x += velocity_x_ * dt;
    rect_.y += velocity_y_ * dt;

    if (rect_.y < 0.0f) {
        rect_.y = 0.0f;
        velocity_y_ = -velocity_y_;
    }

    const float max_y = static_cast<float>(window_height) - rect_.h;
    if (rect_.y > max_y) {
        rect_.y = max_y;
        velocity_y_ = -velocity_y_;
    }
}

void Ball::Render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRectF(renderer, &rect_);
}

void Ball::SetVelocity(float velocity_x, float velocity_y) {
    velocity_x_ = velocity_x;
    velocity_y_ = velocity_y;
}
