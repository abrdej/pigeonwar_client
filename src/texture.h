#pragma once

#include <SDL2/SDL.h>

#include <string>

class Window;

class Texture {
 public:
  explicit Texture(Window& window, const std::string &texture_file);
  void Draw(Window& window);
  void SetPos(int x, int y);
  std::pair<int, int> GetPos() const;
  void Flip(bool flipped);
  void Scale(float factor);
  void SetTransparency(std::uint8_t alpha);

 private:
  SDL_Rect rect_;
  SDL_Point center_;
  SDL_Texture *texture_;
  bool flipped_{false};
  float scaling_factor_{1.f};
};
