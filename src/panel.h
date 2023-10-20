#pragma once

#include <button.h>

#include <iostream>

class Panel {
 public:
  Panel(TextureLoader& texture_loader, int pos_x, int pos_y, int number_of_buttons)
      : texture_loader_(texture_loader), pos_x_(pos_x), pos_y_(pos_y) {
    buttons_.reserve(number_of_buttons);
    for (int n = 0; n < number_of_buttons; ++n) {
      buttons_.emplace_back(texture_loader.GetTexture("border"),
                            pos_x + n * 60,
                            pos_y,
                            60).OnClicked([n](const Button& button) {
        std::cout << "Button: " << n << " clicked\n";
      });
      buttons_order_.emplace_back(ButtonOrderHolder{&buttons_.back()});
    }
  }

  void Draw(Window& window) {
    std::sort(std::begin(buttons_order_), std::end(buttons_order_), Compare{});
    for (auto& button : buttons_order_) {
      button.button->Draw(window);
    }
    if (entity_button_) {
      entity_button_->Draw(window);
    }
  }

  void InteractMove(int x, int y) {
    for (auto& button : buttons_) {
      button.InteractMove(x, y);
    }
  }

  void InteractPress(int x, int y) {
    for (auto& button : buttons_) {
      button.InteractPress(x, y);
    }
  }

  [[nodiscard]] bool Clicked(int x, int y) const {
    return x >= pos_x_ && x <= pos_x_ + buttons_.size() * 60 && y >= pos_y_ && y <= pos_y_ + buttons_.size() * 60;
  }

  void SetEntityButtonTexture(const std::string& texture_key) {
    entity_button_ = std::make_unique<Button>(texture_loader_.GetTexture(texture_key), 15, 600, 120);
  }

 private:
  TextureLoader& texture_loader_;
  int pos_x_{0};
  int pos_y_{0};
  std::unique_ptr<Button> entity_button_;
  std::vector<Button> buttons_;
  std::vector<ButtonOrderHolder> buttons_order_;
};
