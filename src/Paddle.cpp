#include "Paddle.h"

Paddle::Paddle(float x, float y, float width, float height, float speed)
    : rect_{x, y, width, height}, speed_(speed) {}

void Paddle::SetVelocityY(float velocity_y) {
    velocity_y_ = velocity_y;
}

void Paddle::SetY(float y) {
    rect_.y = y;
}

void Paddle::Update(float dt, int window_height) {
    rect_.y += velocity_y_ * dt;

    if (rect_.y < 0.0f) {
        rect_.y = 0.0f;
    }

    const float max_y = static_cast<float>(window_height) - rect_.h;
    if (rect_.y > max_y) {
        rect_.y = max_y;
    }
}

void Paddle::Render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRectF(renderer, &rect_);
}
