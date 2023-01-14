#pragma once

#include <functional>

#include <SDL2/SDL.h>

class Window {
 public:
  using OnMousePress = std::function<void(int, int)>;

  Window();
  ~Window();
  [[nodiscard]] bool IsOpen() const;

  void ProcessEvents();

  void Clear();
  void Display();

  void OnMousePressed(OnMousePress on_mouse_press) {
    on_mouse_press_ = std::move(on_mouse_press);
  }

  [[nodiscard]] SDL_Renderer* GetRenderer() const {
    return renderer_;
  }

 private:
  void MousePress(SDL_MouseButtonEvent& event);

  bool is_open_{true};
  SDL_Window* window_{nullptr};
  SDL_Renderer* renderer_{nullptr};

  OnMousePress on_mouse_press_;
};
