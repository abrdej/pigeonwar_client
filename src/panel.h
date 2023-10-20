#pragma once

#include <button.h>

#include <memory>
#include <vector>

class TextureLoader;

class Panel {
 public:
  Panel(TextureLoader& texture_loader, int pos_x, int pos_y, int number_of_buttons);

  void Draw(Window& window);

  void InteractMove(int x, int y);

  void InteractPress(int x, int y);

  [[nodiscard]] bool Clicked(int x, int y) const;

  void SetEntityButtonTexture(const std::string& texture_key);

 private:
  TextureLoader& texture_loader_;
  int pos_x_{0};
  int pos_y_{0};
  std::unique_ptr<Button> entity_button_;
  std::vector<Button> buttons_;
  std::vector<ButtonOrderHolder> buttons_order_;
};
