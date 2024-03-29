#pragma once

#include <memory>

#include <entity_properties.h>
#include <index_pos_conversion.h>
#include <renderer.h>
#include <text.h>
#include <texture.h>
#include <texture_loader.h>

using BringToTopCallback = std::function<void()>;

class Entity {
 public:
  Entity(Renderer renderer, const TextureLoader& texture_loader, BringToTopCallback bring_to_top_callback,
         const EntityProperties& entity_properties);
  Entity(const Entity&) = delete;
  Entity(Entity&&) = delete;
  Entity& operator=(const Entity&) = delete;
  Entity& operator=(Entity&&) = delete;

  void Draw(Window& window);

  void SetIndex(IndexType index);

  void SetPlayer(PlayerIdType player_id);

  void Flip(bool flip);

  void BringToTop();

  void ChangeHealth(HealthType change_amount);

  void ChangePower(PowerType change_amount);

//  void SetHealth(std::int32_t health);
//  void ChangeHealth(std::int32_t amount);
//  void SetPower(std::int32_t power);
//  void SetColorForHealth();
//  void SetPosition(std::int32_t x, std::int32_t y);

  void ChangeTexture(TextureKey texture_key);

  void RevertTexture();

  std::pair<float, float> GetPos() const;

  EntityProperties GetProperties() const;

  void SetPos(float x, float y);

  void Scale(float factor);

  void SetTransparency(std::uint8_t alpha);

  [[nodiscard]] std::string GetName() const;

  //int GetOrder() const;

 private:
  void UpdateHealthPos();
  void UpdatePowerPos();
  void UpdateHealthStatus();

  const TextureLoader& texture_loader_;
  BringToTopCallback bring_to_top_callback_;

  EntityProperties entity_properties_;

  float x_{0.f};
  float y_{0.f};

  PlayerIdType player_id_{no_player};

  inline static int initial_order = 0;
  int order_{0};
  bool flip_{false};

  TextureKey texture_key_;
  std::unique_ptr<Texture> texture_;
  std::unique_ptr<Texture> texture_backup_;
  std::unique_ptr<Text> health_text_;
  std::unique_ptr<Text> power_text_;
};
