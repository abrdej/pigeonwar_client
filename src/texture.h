#pragma once

#include <SDL2/SDL.h>

#include <string>

#include <window.h>

class Texture {
 public:
  explicit Texture(Window& window, const std::string &texture_file);
  void Draw(Window& window);
  void SetPos(int x, int y);
  void GetPos(int& x, int& y);
  void Flip(bool flipped);

 private:
  SDL_Rect rect_;
  SDL_Point center_;
  SDL_Texture *texture_;
  bool flipped_{false};
};
