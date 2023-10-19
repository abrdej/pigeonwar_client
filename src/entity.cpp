#include <entity.h>

Entity::Entity(SDL_Renderer* renderer, TextureLoader& texture_loader, const EntityProperties& entity_properties) {
// 1. Set fields

// 2. Create sprite: set pos and texture key
  texture_ = std::make_unique<Texture>(texture_loader.GetTexture(entity_properties.name));
  std::tie(x_, y_) = IndexToPos(entity_properties.index);
  texture_->SetPos(x_, y_);

  health_text_ = std::make_unique<Text>(renderer, 24);
  health_text_->SetText(std::to_string(health_));
  health_text_->SetColor(SDL_Color{20, 35, 60});
  UpdateHealthPos();
}

void Entity::Draw(Window& window) {
  // Draw sprite
  texture_->Draw(window);
  // Draw health
  health_text_->Draw(window);

  // Draw power
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
}

void Entity::Scale(float factor) {
  texture_->Scale(factor);
}

void Entity::SetTransparency(std::uint8_t alpha) {
  texture_->SetTransparency(alpha);
}

std::string Entity::GetName() const {
  return name_;
}

void Entity::UpdateHealthPos() {
  health_text_->SetCenterPosX(x_ + 30, y_ + 30 - 55);
}
