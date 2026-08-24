#include "Ball.h"

Ball::Ball(float x, float y, float size, float velocity_x, float velocity_y)
    : rect_{x, y, size, size}, velocity_x_(velocity_x), velocity_y_(velocity_y) {}

void Ball::Update(float dt) {
    rect_.x += velocity_x_ * dt;
    rect_.y += velocity_y_ * dt;
}

void Ball::Render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRectF(renderer, &rect_);
}

void Ball::SetVelocity(float velocity_x, float velocity_y) {
    velocity_x_ = velocity_x;
    velocity_y_ = velocity_y;
}
