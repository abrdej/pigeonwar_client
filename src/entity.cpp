#include <entity.h>

// TODO: configure:
// - health text color
// - health text font size
// - health text pos offset
// - power text color
// - power text font size
// - power text pos offset


Entity::Entity(Renderer renderer, TextureLoader& texture_loader, const EntityProperties& entity_properties)
    : entity_properties_(entity_properties) {
// 1. Set fields

// 2. Create sprite: set pos and texture key
  texture_ = std::make_unique<Texture>(texture_loader.GetTexture(entity_properties.name));
  std::tie(x_, y_) = IndexToPos(entity_properties.index);
  texture_->SetPos(x_, y_);

  if (entity_properties_.health != no_health) {
    health_text_ = std::make_unique<Text>(renderer, 24);
    health_text_->SetText(std::to_string(entity_properties_.health));
    health_text_->SetColor(SDL_Color{20, 35, 60});
    health_text_->DrawBackground(true);
    health_text_->SetBackgroundColor(SDL_Color{141, 166, 146, 25});
    UpdateHealthPos();
  }

  // TODO: do we want to draw power?
//  if (entity_properties_.power != no_power) {
//    power_text_ = std::make_unique<Text>(renderer, 24);
//    power_text_->SetText(std::to_string(entity_properties_.power));
//    power_text_->SetColor(SDL_Color{60, 35, 20});
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

void Entity::Flip(bool flip) {
  texture_->Flip(flip);
}

std::pair<float, float> Entity::GetPos() const {
  return {x_, y_};
}

void Entity::SetPos(float x, float y) {
  x_ = x;
  y_ = y;
  texture_->SetPos(x_, y_);
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

void Entity::UpdateHealthPos() {
  if (health_text_) {
    health_text_->SetCenterPosX(x_ + 30, y_ + 30 - 55);
  }
}

void Entity::UpdatePowerPos() {
//  if (power_text_) {
//    power_text_->SetCenterPosX(x_ + 30, y_ + 30 - 55);
//  }
}
