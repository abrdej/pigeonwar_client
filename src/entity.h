#pragma once

#include <string>
#include <memory>

#include <defs.h>
#include <texture.h>
#include <texture_loader.h>
#include <index_pos_conversion.h>
#include <text.h>
#include <renderer.h>

struct EntityProperties {
  std::string name;
  std::int32_t health{no_health};
  std::int32_t power{no_power};
  std::int32_t index{no_index};
};

class Entity {
 public:
  Entity(Renderer renderer, const TextureLoader& texture_loader, const EntityProperties& entity_properties);
  Entity(const Entity&) = delete;
  Entity(Entity&&) = default;
  Entity& operator=(const Entity&) = delete;
  Entity& operator=(Entity&&) = default;

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
  void UpdatePowerPos();

  EntityProperties entity_properties_;

  float x_{0.f};
  float y_{0.f};

  std::int32_t player_id_{0};
  std::int32_t pos_x_{0};
  std::int32_t pox_y_{0};
  bool is_tweening_{false};

  std::unique_ptr<Texture> texture_;
  std::unique_ptr<Text> health_text_;
  std::unique_ptr<Text> power_text_;
};
