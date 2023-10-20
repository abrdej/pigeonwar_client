#pragma once

#include <cstdint>

#include <SDL2/SDL_pixels.h>

struct Color {
  std::uint8_t r{0};
  std::uint8_t g{0};
  std::uint8_t b{0};
  std::uint8_t a{255};
};

inline SDL_Color ToSDL(const Color& color) {
  return {color.r, color.g, color.b, color.a};
}
