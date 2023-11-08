#pragma once

#include <SDL2/SDL.h>

#include <optional>
#include <string>

class Window;

class Texture {
 public:
  explicit Texture(Window& window, const std::string &texture_file);
  void Draw(Window& window) const;
  void SetPos(int x, int y);
  std::pair<int, int> GetPos() const;
  void Flip(bool flipped);
  void Scale(float factor);
  void SetTransparency(std::uint8_t alpha);
  void SetAnchor(float x, float y);

 private:
  SDL_Rect rect_;
  SDL_FPoint anchor_{0.f, 0.f};
  SDL_Point center_{0, 0};
  SDL_Texture *texture_{nullptr};
  bool flipped_{false};
  float scaling_factor_{1.f};
  std::uint8_t transparency_{255};
};
