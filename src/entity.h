#pragma once

#include <memory>

#include <entity_properties.h>
#include <index_pos_conversion.h>
#include <renderer.h>
#include <text.h>
#include <texture.h>
#include <texture_loader.h>

class Entity {
 public:
  Entity(Renderer renderer, const TextureLoader& texture_loader, const EntityProperties& entity_properties);
  Entity(const Entity&) = delete;
  Entity(Entity&&) = default;
  Entity& operator=(const Entity&) = delete;
  Entity& operator=(Entity&&) = default;

  void Draw(Window& window);

  void SetIndex(IndexType index);

  void Flip(bool flip);

  void BringToTop();

//  void SetHealth(std::int32_t health);
//  void ChangeHealth(std::int32_t amount);
//  void SetPower(std::int32_t power);
//  void ChangePower(std::int32_t amount);
//  void SetColorForHealth();
//  void SetPosition(std::int32_t x, std::int32_t y);
//  void SetPlayer(std::int32_t player_id);

  std::pair<float, float> GetPos() const;

  EntityProperties GetProperties() const;

  void SetPos(float x, float y);

  void Scale(float factor);

  void SetTransparency(std::uint8_t alpha);

  [[nodiscard]] std::string GetName() const;

  int GetOrder() const;

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
  inline static int initial_order = 0;
  int order_{0};

  std::unique_ptr<Texture> texture_;
  std::unique_ptr<Text> health_text_;
  std::unique_ptr<Text> power_text_;
};
