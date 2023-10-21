#pragma once

#include <button.h>
#include <entity_properties.h>
#include <text.h>
#include <timer.h>

#include <memory>
#include <vector>

class TextureLoader;

class Panel {
 public:
  Panel(Renderer renderer, TextureLoader& texture_loader, int pos_x, int pos_y, int number_of_buttons,
        int number_of_effects);

  void Draw(Window& window);

  void InteractMove(int x, int y);

  void InteractPress(int x, int y);

  [[nodiscard]] bool Clicked(int x, int y) const;

  void SetCurrentEntity(const EntityProperties& entity_properties);
  void OnIn(std::function<void(int)> callback);
  void OnOut(std::function<void(int)> callback);

 private:
  Renderer renderer_;
  TextureLoader& texture_loader_;
  int pos_x_{0};
  int pos_y_{0};

  std::function<void(int)> on_in_;
  std::function<void(int)> on_out_;

  std::unique_ptr<Texture> background_;

  std::unique_ptr<Button> entity_button_;
  std::unique_ptr<Text> entity_name_;
  std::unique_ptr<Text> entity_health_;
  std::unique_ptr<Text> entity_power_;

  std::vector<Button> buttons_;
  std::vector<ButtonOrderHolder> buttons_order_;
};
