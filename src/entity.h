#pragma once

#include <string>
#include <memory>

#include <texture.h>
#include <texture_loader.h>
#include <index_pos_conversion.h>
#include <text.h>

struct EntityProperties {
  std::string name;
  std::int32_t health;
  std::int32_t power;
  std::int32_t index;
};

class Entity {
 public:
  Entity(SDL_Renderer* renderer, TextureLoader& texture_loader, const EntityProperties& entity_properties) {
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

  void Draw(Window& window) {
    // Draw sprite
    texture_->Draw(window);
    // Draw health
    health_text_->Draw(window);

    // Draw power
  }

  void Flip(bool flip) {
    texture_->Flip(flip);
  }

//  void SetHealth(std::int32_t health);
//  void ChangeHealth(std::int32_t amount);
//  void SetPower(std::int32_t power);
//  void ChangePower(std::int32_t amount);
//  void SetColorForHealth();
//  void SetPosition(std::int32_t x, std::int32_t y);
//  void SetPlayer(std::int32_t player_id);

  std::pair<float, float> GetPos() const {
    return {x_, y_};
  }

  void SetPos(float x, float y) {
    x_ = x;
    y_ = y;
    texture_->SetPos(x_, y_);
    UpdateHealthPos();
  }

  void Scale(float factor) {
    texture_->Scale(factor);
  }

  void SetTransparency(std::uint8_t alpha) {
    texture_->SetTransparency(alpha);
  }

  [[nodiscard]] std::string GetName() const {
    return name_;
  }

 private:
  void UpdateHealthPos() {
    health_text_->SetCenterPosX(x_ + 30, y_ + 30 - 55);
  }

  std::int32_t player_id_;
  std::int32_t index_;
  std::string name_;
  std::int32_t health_{50};
  std::int32_t power_;
  std::int32_t pos_x_;
  std::int32_t pox_y_;
  bool is_tweening_;

  float x_;
  float y_;

  std::unique_ptr<Texture> texture_;
  std::unique_ptr<Text> health_text_;
};
