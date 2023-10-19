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
  Entity(SDL_Renderer* renderer, TextureLoader& texture_loader, const EntityProperties& entity_properties);

  void Draw(Window& window);

  void Flip(bool flip);

//  void SetHealth(std::int32_t health);
//  void ChangeHealth(std::int32_t amount);
//  void SetPower(std::int32_t power);
//  void ChangePower(std::int32_t amount);
//  void SetColorForHealth();
//  void SetPosition(std::int32_t x, std::int32_t y);
//  void SetPlayer(std::int32_t player_id);

  std::pair<float, float> GetPos() const;

  void SetPos(float x, float y);

  void Scale(float factor);

  void SetTransparency(std::uint8_t alpha);

  [[nodiscard]] std::string GetName() const;

 private:
  void UpdateHealthPos();

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
