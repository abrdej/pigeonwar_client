#include "animation.h"

#include "utils.h"

namespace {

struct TextureGetter {
  explicit TextureGetter(TextureLoader& texture_loader) : texture_loader_(texture_loader) {}
  auto operator()(const std::string& texture_key) const {
    return texture_loader_.GetTexture(texture_key);
  }
  TextureLoader& texture_loader_;
};

}  // namespace

bool QueuedAnimation::Update(std::chrono::milliseconds delta_time) {
  if (animation_queue_.empty()) {
    return true;
  }
  auto& animation = animation_queue_.front();
  if (animation->Update(delta_time)) {
    animation_queue_.pop_front();
  }
  return false;
}

void QueuedAnimation::Draw(Window& window) const {
  if (animation_queue_.empty()) {
    return;
  }
  animation_queue_.front()->Draw(window);
}

void QueuedAnimation::PushBack(std::unique_ptr<AnimationInterface> animation) {
  animation_queue_.emplace_back(std::move(animation));
}

void QueuedAnimation::PushFront(std::unique_ptr<AnimationInterface> animation) {
  animation_queue_.emplace_front(std::move(animation));
}

bool QueuedFactoryAnimation::Update(std::chrono::milliseconds delta_time) {
  if (!current_animation_) {
    if (factories_queue_.empty()) {
      return true;
    }
    std::cout << "Getting next factory from queue\n";
    auto& animation_factory = factories_queue_.front();
    current_animation_ = animation_factory();
    factories_queue_.pop_front();
  }
  if (current_animation_ && current_animation_->Update(delta_time)) {
    std::cout << "Animation finished\n";
    current_animation_ = nullptr;
  }
  return false;
}

void QueuedFactoryAnimation::Draw(Window& window) const {
  if (current_animation_) {
    current_animation_->Draw(window);
  }
}

void QueuedFactoryAnimation::ScheduleBack(AnimationFactory animation_factory) {
  factories_queue_.emplace_back(std::move(animation_factory));
}

void QueuedFactoryAnimation::ScheduleFront(AnimationFactory animation_factory) {
  factories_queue_.emplace_front(std::move(animation_factory));
}

ShotBaseAnimation::ShotBaseAnimation(
    TextureLoader& texture_loader,
    IndexType source_index,
    IndexType target_index,
    float speed,
    const TextureKey& bullet_key,
    const std::optional<TextureKey>& explosion_key,
    const std::optional<std::chrono::milliseconds>& explosion_duration)
    : bullet_texture_(texture_loader.GetTexture(bullet_key)),
      explosion_texture_(AndThen(explosion_key, TextureGetter(texture_loader))),
      explosion_duration_(explosion_duration) {
  auto [source_x, source_y] = IndexToPos(source_index);
  auto [target_x, target_y] = IndexToPos(target_index);
  bullet_texture_->SetAnchor(0.5f, 0.5f);
  bullet_texture_->SetPos(source_x, source_y);
  bullet_texture_->Flip(source_x - target_x > 0);
  move_to_ = std::make_unique<MoveToT>(*bullet_texture_, target_x, target_y, speed);

  if (explosion_texture_ && !explosion_duration_) {
    explosion_texture_.reset();
  }
  if (explosion_texture_ && explosion_duration_) {
    explosion_texture_->SetAnchor(0.5f, 0.5f);
    explosion_texture_->SetPos(target_x, target_y);
  }
}

bool ShotBaseAnimation::Update(std::chrono::milliseconds delta_time) {
  if (move_to_) {
    if (move_to_->Update(delta_time)) {
      bullet_texture_.reset();
      move_to_ = nullptr;
      if (explosion_texture_ && explosion_duration_) {
        explode_until_ = std::chrono::steady_clock::now() + *explosion_duration_;
        return false;
      }
      return true;
    }
  } else if (explosion_texture_) {
    if (std::chrono::steady_clock::now() >= explode_until_) {
      explosion_texture_.reset();
      return true;
    }
  }
  return false;
}

void ShotBaseAnimation::Draw(Window& window) const {
  if (bullet_texture_) {
    bullet_texture_->Draw(window);
  } else if (explosion_texture_) {
    explosion_texture_->Draw(window);
  }
}

// TODO: add support for moving_key
MoveAndReturnBaseAnimation::MoveAndReturnBaseAnimation(Entity& entity,
                                                       IndexType target_index,
                                                       float speed,
                                                       const std::optional<IndexType>& return_index,
                                                       const std::optional<TextureKey>& flush_key,
                                                       const std::optional<TextureKey>& moving_key)
    : entity_(entity), target_index_(target_index), speed_(speed) {

  const auto [from_x, from_y] = entity_.GetPos();
  const auto [target_x, target_y] = IndexToPos(target_index_);
  const auto flip = static_cast<int>(from_x) - target_x > 0;

  ScheduleBack([this]() {
    auto [x, y] = IndexToPos(target_index_);
    return std::make_unique<MoveToT>(entity_, x, y, speed_);
  });

  if (flush_key) {
    ScheduleBack([this, flush_key]() {
      return std::make_unique<FlushTexture>(entity_, *flush_key, std::chrono::milliseconds(200));
    });
  }

  const auto [x, y] = entity_.GetPos();
  ScheduleBack([this, return_index, x_ = x, y_ = y]() mutable {
    if (return_index) {
      std::tie(x_, y_) = IndexToPos(*return_index);
    }
    return std::make_unique<MoveToT>(entity_, x_, y_, speed_);
  });

  ScheduleBack([this, flip]() mutable {
    return std::make_unique<FunctionWrappedAnimation>([this, flip]() {
      entity_.Flip(flip);
      return true;
    });
  });
}

void ScaleAnimation::Handle(Entity& entity) {
  entity_ = &entity;
}

bool ScaleAnimation::Update(std::chrono::milliseconds delta_time) {
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

void TransparencyAnimation::Handle(Entity& entity) {
  entity_ = &entity;
}

bool TransparencyAnimation::Update(std::chrono::milliseconds delta_time) {
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
