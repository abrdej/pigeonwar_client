#include <animation.h>



MoveAnimation::MoveAnimation(Entity& entity, IndexType to_index) {
  auto [x, y] = IndexToPos(to_index);
  move_to_ = std::make_unique<MoveToT>(entity, x, y, speed_);
}

bool MoveAnimation::Update(std::chrono::milliseconds delta_time) {
  return move_to_->Update(delta_time);
}

ChangeHealthAnimation::ChangeHealthAnimation(Renderer renderer, Entity& entity, HealthType change_amount)
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

bool ChangeHealthAnimation::Update(std::chrono::milliseconds delta_time) {
  if (move_by_->Update(delta_time)) {
    entity_->ChangeHealth(change_amount_);
    move_by_ = nullptr;
    text_ = nullptr;
    return true;
  }
  return false;
}

void ChangeHealthAnimation::Draw(Window& window) const {
  if (text_) {
    text_->Draw(window);
  }
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
