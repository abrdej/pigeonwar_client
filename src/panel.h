#pragma once

#include <texture_loader.h>
#include <window.h>

#include <iostream>
#include <set>

class Button {
 public:
  Button(Texture texture, int x, int y, int size)
      : texture_(texture), x_(x), y_(y), size_(size) {
    texture_.SetPos(x, y);

    OnIn([this]() {
      texture_.Scale(1.2);
      std::cout << "OnIn\n";
    });
    OnOut([this]() {
      texture_.Scale(1.0);
      std::cout << "OnOut\n";
    });
  }

  [[nodiscard]] bool IsHovered(int x, int y) const {
    return x >= x_ && x <= x_ + size_ && y >= y_ && y <= y_ + size_;
  }

  void Draw(Window& window) {
    texture_.Draw(window);
  }

  void OnIn(const std::function<void()>& callback) {
    on_in_ = callback;
  }

  void OnOut(const std::function<void()>& callback) {
    on_out_ = callback;
  }

   void OnClicked(const std::function<void(const Button&)> callback) {
     on_clicked_ = callback;
  }

  void InteractMove(int x, int y) {
    if (!is_hovered_ && IsHovered(x, y)) {
      if (on_in_) {
        on_in_();
      }
      order_ = 100;
      is_hovered_ = true;
    } else if (is_hovered_ && !IsHovered(x, y)) {
      if (on_out_) {
        on_out_();
      }
      is_hovered_ = false;
      order_ = 0;
    }
  }

  void InteractPress(int x, int y) {
    if (is_hovered_) {
      on_clicked_(*this);
    }
  }

  int GetOrder() const {
    return order_;
  }

 private:
  Texture texture_;
  int x_{0};
  int y_{0};
  int size_{0};
  std::function<void()> on_in_;
  std::function<void()> on_out_;
  std::function<void(const Button&)> on_clicked_;
  bool is_hovered_{false};
  int order_{0};
};

struct ButtonOrderHolder {
  Button* button;
};

struct Compare {
  bool operator()(const ButtonOrderHolder& a, const ButtonOrderHolder& b) const {
    return a.button->GetOrder() < b.button->GetOrder();
  }
};

class Panel {
 public:
  Panel(TextureLoader& texture_loader, int pos_x, int pos_y, int number_of_buttons)
      : pos_x_(pos_x), pos_y_(pos_y) {
    buttons_.reserve(number_of_buttons);
    for (int n = 0; n < number_of_buttons; ++n) {
      buttons_.emplace_back(texture_loader.GetTexture("border"), pos_x + n * 60, pos_y, 60);
      buttons_order_.emplace_back(ButtonOrderHolder{&buttons_.back()});
      buttons_.back().OnClicked([n](const Button& button) {
        std::cout << "Button: " << n << " clicked\n";
      });
    }
  }

  void Draw(Window& window) {
    std::sort(std::begin(buttons_order_), std::end(buttons_order_), Compare{});
    for (auto& button : buttons_order_) {
      button.button->Draw(window);
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

 private:
  int pos_x_{0};
  int pos_y_{0};
  std::vector<Button> buttons_;
  std::vector<ButtonOrderHolder> buttons_order_;
};
