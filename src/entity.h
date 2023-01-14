#pragma once

#include <string>
#include <memory>

#include <texture.h>
#include <texture_loader.h>

struct EntityProperties {
  std::string name;
  std::int32_t health;
  std::int32_t power;
  std::int32_t index;
};

inline void IndexToPos(std::int32_t index, int& x, int& y) {
  int col = index % 15;
  int row = index / 15;
  x = col * 64;
  y = row * 64;
}

class Entity {
 public:
  Entity(TextureLoader& texture_loader, const EntityProperties& entity_properties) {
    // 1. Set fields

    // 2. Create sprite: set pos and texture key
    texture_ = std::make_unique<Texture>(texture_loader.GetTexture(entity_properties.name));
    int x, y;
    IndexToPos(entity_properties.index, x, y);
    texture_->SetPos(x, y);
  }

  void Draw(Window& window) {
    // Draw sprite
    texture_->Draw(window);

    // Draw health
    // Draw power
  }

//  void Flip();
//  void SetHealth(std::int32_t health);
//  void ChangeHealth(std::int32_t amount);
//  void SetPower(std::int32_t power);
//  void ChangePower(std::int32_t amount);
//  void SetColorForHealth();
//  void SetPosition(std::int32_t x, std::int32_t y);
//  void SetPlayer(std::int32_t player_id);

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

  std::unique_ptr<Texture> texture_;
};
