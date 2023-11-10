#include <entity.h>

// TODO: configure:
// - health text color
// - health text font size
// - health text pos offset
// - power text color
// - power text font size
// - power text pos offset

Color GetHealthBackgroundColorForPlayer(PlayerIdType player_id) {
  if (player_id == 0) {
    return {173, 204, 176, 15};
  } else if (player_id == 1) {
    return {173, 190, 204, 15};
  } else {
    return {224, 224, 224, 15};
  }
}

Entity::Entity(Renderer renderer, const TextureLoader& texture_loader, BringToTopCallback bring_to_top_callback,
               const EntityProperties& entity_properties)
    : texture_loader_(texture_loader), bring_to_top_callback_(std::move(bring_to_top_callback)),
      entity_properties_(entity_properties), texture_key_(entity_properties.name) {
// 1. Set fields

// 2. Create sprite: set pos and texture key
  texture_ = std::make_unique<Texture>(texture_loader_.GetTexture(texture_key_));
  std::tie(x_, y_) = IndexToPos(entity_properties.index);
  texture_->SetAnchor(0.5f, 0.5f);
  texture_->SetPos(static_cast<int>(x_), static_cast<int>(y_));

  if (entity_properties_.health != no_health) {
    health_text_ = std::make_unique<Text>(renderer, 24);
    health_text_->SetText(std::to_string(entity_properties_.health));
    health_text_->SetColor(Color{20, 35, 60});
    health_text_->DrawBackground(true);
    health_text_->SetBackgroundColor(GetHealthBackgroundColorForPlayer(no_player));
    UpdateHealthPos();
  }

  // TODO: do we want to draw power?
//  if (entity_properties_.power != no_power) {
//    power_text_ = std::make_unique<Text>(renderer, 24);
//    power_text_->SetText(std::to_string(entity_properties_.power));
//    power_text_->SetColor(Color{60, 35, 20});
//    UpdatePowerPos();
//  }
}

void Entity::Draw(Window& window) {
  // Draw sprite
  texture_->Draw(window);
  // Draw health
  if (health_text_) {
    health_text_->Draw(window);
  }
  // Draw power
//  if (power_text_) {
//    power_text_->Draw(window);
//  }
}

void Entity::SetIndex(IndexType index) {
  entity_properties_.index = index;
  auto [x, y] = IndexToPos(index);
  SetPos(static_cast<float>(x), static_cast<float>(y));
}

void Entity::SetPlayer(PlayerIdType player_id) {
  player_id_ = player_id;
  if (health_text_) {
    health_text_->SetBackgroundColor(GetHealthBackgroundColorForPlayer(player_id));
  }
}

void Entity::Flip(bool flip) {
  flip_ = flip;
  texture_->Flip(flip);
}

void Entity::BringToTop() {
  //order_ = ++initial_order;
  bring_to_top_callback_();
}

void Entity::ChangeHealth(HealthType change_amount) {
  entity_properties_.health += change_amount;
  UpdateHealthStatus();
}

void Entity::ChangeTexture(TextureKey texture_key) {
  texture_backup_ = std::move(texture_);
  texture_key_ = std::move(texture_key);
  texture_ = std::make_unique<Texture>(texture_loader_.GetTexture(texture_key_));
  texture_->SetAnchor(0.5f, 0.5f);
  texture_->SetPos(static_cast<int>(x_), static_cast<int>(y_));
  texture_->Flip(flip_);
}

void Entity::RevertTexture() {
  if (texture_backup_) {
    texture_ = std::move(texture_backup_);
    texture_->SetAnchor(0.5f, 0.5f);
    texture_->SetPos(static_cast<int>(x_), static_cast<int>(y_));
    texture_->Flip(flip_);
  }
}

std::pair<float, float> Entity::GetPos() const {
  return {x_, y_};
}

EntityProperties Entity::GetProperties() const {
  return entity_properties_;
}

void Entity::SetPos(float x, float y) {
  x_ = x;
  y_ = y;
  texture_->SetPos(static_cast<int>(x_), static_cast<int>(y_));
  UpdateHealthPos();
  UpdatePowerPos();
}

void Entity::Scale(float factor) {
  texture_->Scale(factor);
}

void Entity::SetTransparency(std::uint8_t alpha) {
  texture_->SetTransparency(alpha);
}

std::string Entity::GetName() const {
  return entity_properties_.name;
}

//int Entity::GetOrder() const {
//  return order_;
//}

void Entity::UpdateHealthPos() {
  if (health_text_) {
    health_text_->SetAnchor(0.5, 0.0);
    health_text_->SetPos(static_cast<int>(x_), static_cast<int>(y_) - 55);
  }
}

void Entity::UpdatePowerPos() {
//  if (power_text_) {
//    power_text_->SetAnchor(0.5, 0.0);
//    power_text_->SetPos(x_, y_ - 55);
//  }
}

void Entity::UpdateHealthStatus() {
  if (health_text_) {
    health_text_->SetText(std::to_string(entity_properties_.health));
  }
}
