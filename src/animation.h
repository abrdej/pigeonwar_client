#pragma once

#include <chrono>
#include <cstdint>

#include <entity.h>
#include <iostream>

inline double Distance(int from_x, int from_y, int to_x, int to_y) {
  return std::sqrt(std::pow(from_x - to_x, 2) + std::pow(from_y - to_y, 2));
}

template <typename T, bool flip, bool bring_to_top>
class MoveTo {
 public:
  MoveTo(T& movable, int x, int y, float speed)
      : movable_(movable), target_x_(x), target_y_(y), speed_(speed) {
    auto [from_x, from_y] = movable_.GetPos();
    if constexpr (flip) {
      movable.Flip(from_x - target_x_ > 0);
    }
    if constexpr (bring_to_top) {
      movable.BringToTop();
    }
    auto distance = Distance(from_x, from_y, target_x_, target_y_);
    delta_x_ = (target_x_ - from_x);
    delta_y_ = (target_y_ - from_y);
    delta_x_ = delta_x_ / distance;
    delta_y_ = delta_y_ / distance;
  }

  bool Update(std::chrono::milliseconds delta_time) {
    auto [x, y] = movable_.GetPos();

    auto pos_x = x + speed_ * delta_x_ * static_cast<float>(delta_time.count());
    auto pos_y = y + speed_ * delta_y_ * static_cast<float>(delta_time.count());

    if (std::abs(pos_x - target_x_) < 2.5 && std::abs(pos_y - target_y_) < 2.5) {
      movable_.SetPos(target_x_, target_y_);
      return true;
    } else {
      movable_.SetPos(pos_x, pos_y);
      return false;
    }
  }

 private:
  T& movable_;
  int target_x_{0};
  int target_y_{0};
  float delta_x_{0.f};
  float delta_y_{0.f};
  float speed_{0.f};
};

template <typename T, bool flip, bool bring_to_top>
class MoveBy {
 public:
  MoveBy(T& movable, int delta_x, int delta_y, float speed)
      : movable_(movable),
        delta_x_(delta_x),
        delta_y_(delta_y),
        speed_x_(delta_x_ * speed),
        speed_y_(delta_y_ * speed) {
    std::tie(initial_x_, initial_y_) = movable_.GetPos();
    if constexpr (flip) {
      movable.Flip(delta_x_ > 0);
    }
    if constexpr (bring_to_top) {
      movable.BringToTop();
    }
  }

  bool Update(std::chrono::milliseconds delta_time) {
    auto [x, y] = movable_.GetPos();
    auto new_x = x + speed_x_ * static_cast<float>(delta_time.count());
    auto new_y = y + speed_y_ * static_cast<float>(delta_time.count());

    std::cout << "speed_y_: " << speed_y_ << "\n";
    std::cout << "new_y: " << new_y << "\n";

    movable_.SetPos(new_x, new_y);
    if (std::abs(new_x - initial_x_) >= std::abs(delta_x_) && std::abs(new_y - initial_y_) >= std::abs(delta_y_)) {
      movable_.SetPos(initial_x_ + delta_x_, initial_y_ + delta_y_);
      return true;
    }
    return false;
  }

 private:
  T& movable_;
  int initial_x_{0};
  int initial_y_{0};
  int delta_x_{0};
  int delta_y_{0};
  float speed_x_{0.f};
  float speed_y_{0.f};
};

class MoveAnimation {
  using MoveToT = MoveTo<Entity, true, true>;
 public:
  explicit MoveAnimation(Entity& entity, IndexType to_index) {
    auto [x, y] = IndexToPos(to_index);
    move_to_ = std::make_unique<MoveToT>(entity, x, y, speed_);
  }

  bool Update(std::chrono::milliseconds delta_time) {
    return move_to_->Update(delta_time);
  }

 private:
  std::unique_ptr<MoveToT> move_to_;
  float speed_{0.02f};
};

class ChangeHealthAnimation {
  using MoveByT = MoveBy<Text, false, false>;
 public:
  explicit ChangeHealthAnimation(Renderer renderer, Entity& entity, HealthType change_amount)
      : renderer_(renderer), entity_(&entity), change_amount_(change_amount),
        color_(change_amount < 0 ? Color{173, 32, 14, 255} : Color{47, 117, 66, 255}) {

    auto [x, y] = entity.GetPos();
    // TODO: this should not be magic number here
    x += 30;
    y -= 30;

    text_ = std::make_unique<Text>(renderer_, 32); // TODO: Parametrize size
    text_->SetText(std::to_string(change_amount_));
    text_->SetCenterPosX(x, y);
    text_->SetColor(color_);

    move_by_ = std::make_unique<MoveByT>(*text_, 0, -25, 0.0002f);
  }

  bool Update(std::chrono::milliseconds delta_time) {
    if (move_by_->Update(delta_time)) {
      entity_->ChangeHealth(change_amount_);
      move_by_ = nullptr;
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
  std::unique_ptr<MoveByT> move_by_;
  Entity* entity_{nullptr};
  HealthType change_amount_;
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
