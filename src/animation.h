#pragma once

#include <chrono>
#include <cstdint>

#include <entity.h>
#include <iostream>

inline double Distance(int from_x, int from_y, int to_x, int to_y) {
  return std::sqrt(std::pow(from_x - to_x, 2) + std::pow(from_y - to_y, 2));
}

struct MoveAnimation {
  int entity_id;
  int to_index;

  float delta_x;
  float delta_y;

  float to_x;
  float to_y;

  float speed = 0.02; // TODO: depending on the speed this is different - fix it

  Entity* entity_{nullptr};

  void Handle(Entity& entity) {
    entity_ = &entity;

    std::tie(to_x, to_y) = IndexToPos(to_index);

    auto [from_x, from_y] = entity.GetPos();

    auto distance = Distance(from_x, from_y, to_x, to_y);

    entity.Flip(from_x - to_x > 0);

    entity.BringToTop();

    delta_x = (to_x - from_x);
    delta_y = (to_y - from_y);

    delta_x = delta_x / distance;
    delta_y = delta_y / distance;
  }

  bool Update(std::chrono::milliseconds delta_time) {
    auto [x, y] = entity_->GetPos();

    auto pos_x = x + speed * delta_x * delta_time.count();
    auto pos_y = y + speed * delta_y * delta_time.count();

    if (std::abs(pos_x - to_x) < 2.5 && std::abs(pos_y - to_y) < 2.5) {
      entity_->SetPos(to_x, to_y);
      return true;
    } else {
      entity_->SetPos(pos_x, pos_y);
      return false;
    }
  }
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
