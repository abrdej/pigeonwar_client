#pragma once

#include <animation_interface.h>
#include <entity.h>

#include <chrono>
#include <cstdint>
#include <iostream>
#include <list>

inline double Distance(int from_x, int from_y, int to_x, int to_y) {
  return std::sqrt(std::pow(from_x - to_x, 2) + std::pow(from_y - to_y, 2));
}

class FunctionWrappedAnimation : public DummyDrawAnimation {
 public:
  using Function = std::function<bool()>;

  explicit FunctionWrappedAnimation(Function f) : f_(std::move(f)) {}

  bool Update(std::chrono::milliseconds delta_time) override {
    return f_();
  }

 private:

  Function  f_;
};

template <typename T, bool flip, bool bring_to_top>
class MoveTo : public DummyDrawAnimation {
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

  bool Update(std::chrono::milliseconds delta_time) override {
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
class MoveBy : public DummyDrawAnimation {
 public:
  MoveBy(T& movable, int delta_x, int delta_y, float speed)
      : movable_(movable),
        delta_x_(delta_x),
        delta_y_(delta_y),
        speed_x_(delta_x_ * speed * 0.001),
        speed_y_(delta_y_ * speed * 0.001) {
    std::tie(initial_x_, initial_y_) = movable_.GetPos();
    if constexpr (flip) {
      movable.Flip(delta_x_ > 0);
    }
    if constexpr (bring_to_top) {
      movable.BringToTop();
    }
  }

  bool Update(std::chrono::milliseconds delta_time) override {
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

class QueuedAnimation : public AnimationInterface {
 public:
  bool Update(std::chrono::milliseconds delta_time) override;
  void Draw(Window& window) const override;

 protected:
  void PushBack(std::unique_ptr<AnimationInterface> animation);
  void PushFront(std::unique_ptr<AnimationInterface> animation);

 private:
  std::list<std::unique_ptr<AnimationInterface>> animation_queue_;
};

class QueuedFactoryAnimation : public AnimationInterface {
 public:
  bool Update(std::chrono::milliseconds delta_time) override;
  void Draw(Window& window) const override;

 protected:
  using AnimationFactory = std::function<std::unique_ptr<AnimationInterface>()>;

  void ScheduleBack(AnimationFactory animation_factory);
  void ScheduleFront(AnimationFactory animation_factory);

 private:
  std::unique_ptr<AnimationInterface> current_animation_;
  std::list<AnimationFactory> factories_queue_;
};

class ShowTexture : public AnimationInterface {
 public:
  ShowTexture(const Texture& texture, std::chrono::milliseconds duration_ms, int x, int y)
      : texture_(texture), duration_ms_(duration_ms) {
    texture_.SetAnchor(0.5f, 0.5f);
    texture_.SetPos(x, y);
  }

  bool Update(std::chrono::milliseconds delta_time) override {
    if (!keep_until_) {
      keep_until_ = std::chrono::steady_clock::now() + duration_ms_;
    }
    return std::chrono::steady_clock::now() >= *keep_until_;
  }

  void Draw(Window& window) const override {
    texture_.Draw(window);
  }

 private:
  Texture texture_;
  std::chrono::milliseconds duration_ms_;
  std::optional<std::chrono::steady_clock::time_point> keep_until_;
};

class FlushTexture : public DummyDrawAnimation {
 public:
  FlushTexture(Entity& entity, TextureKey texture, std::chrono::milliseconds duration_ms)
      : entity_(entity), texture_(std::move(texture)), duration_ms_(duration_ms) {
  }

  bool Update(std::chrono::milliseconds delta_time) override {
    if (!keep_until_) {
      keep_until_ = std::chrono::steady_clock::now() + duration_ms_;
      entity_.ChangeTexture(texture_);
    }
    if (std::chrono::steady_clock::now() >= *keep_until_) {
      entity_.RevertTexture();
      return true;
    }
    return false;
  }

 private:
  Entity& entity_;
  TextureKey texture_;
  TextureKey texture_backup_;
  std::chrono::milliseconds duration_ms_;
  std::optional<std::chrono::steady_clock::time_point> keep_until_;
};

class ShotBaseAnimation : public AnimationInterface {
  using MoveToT = MoveTo<Texture, true, false>;
 public:
  ShotBaseAnimation(TextureLoader& texture_loader,
                    IndexType source_index,
                    IndexType target_index,
                    float speed,
                    const TextureKey& bullet_key,
                    const std::optional<TextureKey>& explosion_key = std::nullopt,
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

class MoveAndReturnBaseAnimation : public QueuedFactoryAnimation {
  using MoveToT = MoveTo<Entity, true, true>;

 public:
  MoveAndReturnBaseAnimation(Entity& entity,
                             IndexType target_index,
                             float speed,
                             const std::optional<IndexType>& return_index = std::nullopt,
                             const std::optional<TextureKey>& flush_key = std::nullopt,
                             const std::optional<TextureKey>& moving_key = std::nullopt);

 private:
  Entity& entity_;
  IndexType target_index_;
  float speed_{0.01f};
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
