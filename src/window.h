#pragma once

#include <SDL2/SDL.h>

class Window {
 public:
  Window();
  ~Window();
  [[nodiscard]] bool IsOpen() const;

  void ProcessEvents();

  void Clear();
  void Display();

  [[nodiscard]] SDL_Renderer* GetRenderer() const {
    return renderer_;
  }

 private:
  void MousePress(SDL_MouseButtonEvent& event);

  bool is_open_{true};
  SDL_Window* window_{nullptr};
  SDL_Renderer* renderer_{nullptr};
};
