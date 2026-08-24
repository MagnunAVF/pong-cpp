#include "Ball.h"

namespace {
constexpr float kMaxDeflectionSpeed = 200.0f;
}  // namespace

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

void Ball::BounceOffPaddle(const SDL_FRect& paddle_rect) {
    velocity_x_ = -velocity_x_;

    const float paddle_center_y = paddle_rect.y + paddle_rect.h / 2.0f;
    const float ball_center_y = rect_.y + rect_.h / 2.0f;

    float offset = (ball_center_y - paddle_center_y) / (paddle_rect.h / 2.0f);
    if (offset < -1.0f) {
        offset = -1.0f;
    } else if (offset > 1.0f) {
        offset = 1.0f;
    }

    velocity_y_ += offset * kMaxDeflectionSpeed;
}
