#pragma once

#include <texture.h>

#include <functional>

class Window;

class Button {
 public:
  Button(Texture texture, int x, int y, int size);

  [[nodiscard]] bool IsHovered(int x, int y) const;

  void Draw(Window& window);

  void OnIn(const std::function<void(Button&)>& callback);

  void OnOut(const std::function<void(Button&)>& callback);

  void OnClicked(const std::function<void(Button&)> callback);

  void InteractMove(int x, int y);

  void InteractPress(int x, int y);

  int GetOrder() const;

  Texture& GetTexture();

 private:
  Texture texture_;
  int x_{0};
  int y_{0};
  int size_{0};
  std::function<void(Button&)> on_in_;
  std::function<void(Button&)> on_out_;
  std::function<void(Button&)> on_clicked_;
  bool is_hovered_{false};
  int order_{0};
};

struct ButtonOrderHolder {
  Button* button{nullptr};
};

struct Compare {
  bool operator()(const ButtonOrderHolder& a, const ButtonOrderHolder& b) const {
    return a.button->GetOrder() < b.button->GetOrder();
  }
};
