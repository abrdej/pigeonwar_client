#pragma once

#include <chrono>
#include <cstdint>

#include <entity.h>
#include <iostream>

inline double Distance(int from_x, int from_y, int to_x, int to_y) {
  return std::sqrt(std::pow(from_x - to_x, 2) + std::pow(from_y - to_y, 2));
}

class MoveAnimation {
 public:
  explicit MoveAnimation(IndexType to_index) : to_index_(to_index) {}

  void Handle(Entity& entity) {
    entity_ = &entity;

    std::tie(to_x_, to_y_) = IndexToPos(to_index_);

    auto [from_x, from_y] = entity.GetPos();

    auto distance = Distance(from_x, from_y, to_x_, to_y_);

    entity.Flip(from_x - to_x_ > 0);

    entity.BringToTop();

    delta_x_ = (to_x_ - from_x);
    delta_y_ = (to_y_ - from_y);

    delta_x_ = delta_x_ / distance;
    delta_y_ = delta_y_ / distance;
  }

  bool Update(std::chrono::milliseconds delta_time) {
    auto [x, y] = entity_->GetPos();

    auto pos_x = x + speed_ * delta_x_ * delta_time.count();
    auto pos_y = y + speed_ * delta_y_ * delta_time.count();

    if (std::abs(pos_x - to_x_) < 2.5 && std::abs(pos_y - to_y_) < 2.5) {
      entity_->SetPos(to_x_, to_y_);
      return true;
    } else {
      entity_->SetPos(pos_x, pos_y);
      return false;
    }
  }

 private:
  int to_index_;
  float delta_x_;
  float delta_y_;
  float to_x_;
  float to_y_;
  float speed_ = 0.02; // TODO: depending on the speed this is different - fix it
  Entity* entity_{nullptr};
};

class ChangeHealthAnimation {
 public:
  explicit ChangeHealthAnimation(Renderer renderer, HealthType change_amount)
      : renderer_(renderer), change_amount_(change_amount) {
    color_ = change_amount < 0 ? Color{173, 32, 14, 255} : Color{47, 117, 66, 255};
  }

  void Handle(Entity& entity) {
    entity_ = &entity;
    std::tie(initial_x_, initial_y_) = entity.GetPos();
    // TODO: this should not be magic number here
    initial_x_ += 15;
    initial_y_ -= 30;
    current_y_ = initial_y_;
    text_ = std::make_unique<Text>(renderer_, 32); // TODO: Parametrize size
    text_->SetText(std::to_string(change_amount_));
    text_->SetCenterPosX(initial_x_, initial_y_);
    text_->SetColor(color_);
  }

  bool Update(std::chrono::milliseconds delta_time) {
    auto new_y = current_y_ - speed_ * delta_time.count();
    text_->SetPos(initial_x_, new_y);
    current_y_ = new_y;
    if (std::abs(current_y_ - initial_y_) > 25.f) {
      entity_->ChangeHealth(change_amount_);
      text_ = nullptr;
      return true;
    }
    return false;
  }

  void Draw(Window& window) {
    if (text_) {
      text_->Draw(window);
    }
  }

 private:
  Renderer renderer_;
  Entity* entity_{nullptr};
  HealthType change_amount_;
  float initial_x_{0.f};
  float initial_y_{0.f};
  float current_y_{0.f};
  float speed_{0.01f};
  Color color_;
  std::unique_ptr<Text> text_;
};

struct ScaleAnimation {
  void Handle(Entity& entity) {
    entity_ = &entity;
  }

  bool Update(std::chrono::milliseconds delta_time) {
    update_delta_time_ += delta_time;
    if (update_delta_time_ >= update_period_) {
      update_delta_time_ -= update_period_;
      current_scale_ += scaling_speed_;
      entity_->Scale(current_scale_);
      if (current_scale_ >= 1.3) {
        scaling_speed_ = -0.001f;
        return false;
      }
      if (current_scale_ <= 1.f) {
        entity_->Scale(1.f);
        return true;
      }
    }
    return false;
  }

  Entity* entity_{nullptr};
  float current_scale_{1.f};
  float scaling_speed_{0.001f};
  std::chrono::milliseconds update_delta_time_;
  std::chrono::milliseconds update_period_{100};
};

struct TransparencyAnimation {
  void Handle(Entity& entity) {
    entity_ = &entity;
  }

  bool Update(std::chrono::milliseconds delta_time) {
    update_delta_time_ += delta_time;
    if (update_delta_time_ >= update_period_) {
      update_delta_time_ -= update_period_;
      current_alpha_ += alpha_step_;
      entity_->SetTransparency(current_alpha_);
      if (current_alpha_ == 0) {
        alpha_step_ = 1;
        return false;
      }
      if (current_alpha_ == 255) {
        return true;
      }
    }
    return false;
  }

  Entity* entity_{nullptr};
  std::uint8_t current_alpha_{255};
  std::int8_t alpha_step_{-1};
  std::chrono::milliseconds update_delta_time_;
  std::chrono::milliseconds update_period_{100};
};
