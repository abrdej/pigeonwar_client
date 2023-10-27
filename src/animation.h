#pragma once

#include <drawable.h>
#include <entity.h>
#include <updatable.h>

#include <chrono>
#include <cstdint>
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

    auto movement_x = speed_ * delta_x_ * static_cast<float>(delta_time.count());
    auto movement_y = speed_ * delta_y_ * static_cast<float>(delta_time.count());

    if (std::abs(x - target_x_) <= std::abs(movement_x) && std::abs(y - target_y_) <= std::abs(movement_y)) {
      movable_.SetPos(target_x_, target_y_);
      return true;
    } else {
      movable_.SetPos(x + movement_x, y + movement_y);
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

struct AnimationInterface : UpdatableInterface, DrawableInterface {};

struct DummyDrawAnimation : AnimationInterface {
  void Draw(Window& window) const final {}
};

class MoveAnimation : public DummyDrawAnimation {
  using MoveToT = MoveTo<Entity, true, true>;
 public:
  explicit MoveAnimation(Entity& entity, IndexType target_index);
  bool Update(std::chrono::milliseconds delta_time) override;

 private:
  Entity& entity_;
  IndexType target_index_;
  std::unique_ptr<MoveToT> move_to_;
  float speed_{0.01f};
};

class ChangeHealthAnimation : public AnimationInterface {
  using MoveByT = MoveBy<Text, false, false>;
 public:
  explicit ChangeHealthAnimation(Renderer renderer, Entity& entity, HealthType change_amount);
  bool Update(std::chrono::milliseconds delta_time) override;
  void Draw(Window& window) const override;

 private:
  Renderer renderer_;
  std::unique_ptr<MoveByT> move_by_;
  Entity& entity_;
  HealthType change_amount_;
  Color color_;
  std::unique_ptr<Text> text_;
};

class ShotBaseAnimation : public AnimationInterface {
  using MoveToT = MoveTo<Texture, true, false>;
 public:
  ShotBaseAnimation(TextureLoader& texture_loader,
                    IndexType source_index,
                    IndexType target_index,
                    float speed,
                    const std::string& bullet_key,
                    const std::optional<std::string>& explosion_key = std::nullopt,
                    const std::optional<std::chrono::milliseconds>& explosion_duration = std::nullopt);
  bool Update(std::chrono::milliseconds delta_time) override;
  void Draw(Window& window) const override;

 private:
  std::unique_ptr<MoveToT> move_to_;
  std::optional<Texture> bullet_texture_;
  std::optional<Texture> explosion_texture_;
  std::optional<std::chrono::milliseconds> explosion_duration_;
  std::chrono::steady_clock::time_point explode_until_;
};

struct ShotAnimation : ShotBaseAnimation {
  ShotAnimation(TextureLoader& texture_loader,
                IndexType source_index,
                IndexType target_index);
};

struct ScaleAnimation {
  void Handle(Entity& entity);

  bool Update(std::chrono::milliseconds delta_time);

  Entity* entity_{nullptr};
  float current_scale_{1.f};
  float scaling_speed_{0.001f};
  std::chrono::milliseconds update_delta_time_;
  std::chrono::milliseconds update_period_{100};
};

struct TransparencyAnimation {
  void Handle(Entity& entity);

  bool Update(std::chrono::milliseconds delta_time);

  Entity* entity_{nullptr};
  std::uint8_t current_alpha_{255};
  std::int8_t alpha_step_{-1};
  std::chrono::milliseconds update_delta_time_;
  std::chrono::milliseconds update_period_{100};
};
