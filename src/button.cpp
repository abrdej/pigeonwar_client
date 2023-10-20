#include <button.h>

#include <window.h>

#include <iostream>

Button::Button(Texture texture, int x, int y, int size)
    : texture_(texture), x_(x), y_(y), size_(size) {
  texture_.SetPos(x, y);
}

bool Button::IsHovered(int x, int y) const {
  return x >= x_ && x <= x_ + size_ && y >= y_ && y <= y_ + size_;
}

void Button::Draw(Window& window) {
  texture_.Draw(window);
}

void Button::OnIn(const std::function<void(Button&)>& callback) {
  on_in_ = callback;
}

void Button::OnOut(const std::function<void(Button&)>& callback) {
  on_out_ = callback;
}

void Button::OnClicked(const std::function<void(Button&)> callback) {
  on_clicked_ = callback;
}

void Button::InteractMove(int x, int y) {
  if (!is_hovered_ && IsHovered(x, y)) {
    if (on_in_) {
      on_in_(*this);
    }
    order_ = 100;
    is_hovered_ = true;
  } else if (is_hovered_ && !IsHovered(x, y)) {
    if (on_out_) {
      on_out_(*this);
    }
    is_hovered_ = false;
    order_ = 0;
  }
}

void Button::InteractPress(int x, int y) {
  if (is_hovered_) {
    on_clicked_(*this);
  }
}

int Button::GetOrder() const {
  return order_;
}

Texture& Button::GetTexture() {
  return texture_;
}
