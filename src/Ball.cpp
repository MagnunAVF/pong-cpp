#include "Ball.h"

#include <cstdlib>

namespace {
constexpr float kMaxDeflectionSpeed = 200.0f;
constexpr float kMaxLaunchAngleRatio = 0.5f;
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

void Ball::Reset(float x, float y, float speed) {
    rect_.x = x;
    rect_.y = y;

    const float direction_x = (std::rand() % 2 == 0) ? 1.0f : -1.0f;
    const float angle_ratio =
        (static_cast<float>(std::rand() % 200) / 100.0f - 1.0f) *
        kMaxLaunchAngleRatio;

    velocity_x_ = direction_x * speed;
    velocity_y_ = angle_ratio * speed;
}
