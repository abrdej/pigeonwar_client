#pragma once

#include <string>
#include <memory>

#include <texture.h>
#include <texture_loader.h>
#include <index_pos_conversion.h>

struct EntityProperties {
  std::string name;
  std::int32_t health;
  std::int32_t power;
  std::int32_t index;
};

class Entity {
 public:
  Entity(TextureLoader& texture_loader, const EntityProperties& entity_properties) {
    // 1. Set fields

    // 2. Create sprite: set pos and texture key
    texture_ = std::make_unique<Texture>(texture_loader.GetTexture(entity_properties.name));
    int x, y;
    IndexToPos(entity_properties.index, x, y);
    texture_->SetPos(x, y);

    x_ = x;
    y_ = y;
  }

  void Draw(Window& window) {
    // Draw sprite
    texture_->Draw(window);

    // Draw health
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

  void GetPos(float& x, float& y) {
    x = x_;
    y = y_;
  }

  void SetPos(float x, float y) {
    x_ = x;
    y_ = y;
    texture_->SetPos(x_, y_);
  }

  [[nodiscard]] std::string GetName() const {
    return name_;
  }

 private:
  std::int32_t player_id_;
  std::int32_t index_;
  std::string name_;
  std::int32_t health_;
  std::int32_t power_;
  std::int32_t pos_x_;
  std::int32_t pox_y_;
  bool is_tweening_;

  float x_;
  float y_;

  std::unique_ptr<Texture> texture_;
};
