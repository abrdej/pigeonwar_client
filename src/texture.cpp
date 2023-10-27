#include <texture.h>

#include <window.h>

#include <iostream>
#include <string>
#include <stdexcept>

#include <SDL2/SDL_image.h>

Texture::Texture(Window& window, const std::string &texture_file) {
  SDL_Surface *image = IMG_Load(texture_file.c_str());
  if (!image) {
    std::cout << "No texture: " << texture_file << "\n";
    throw std::runtime_error("Texture: " + std::string(IMG_GetError()));
  }
  texture_ = SDL_CreateTextureFromSurface(window.GetRenderer().renderer, image);
  rect_.x = 0;
  rect_.y = 0;
  rect_.w = image->w;
  rect_.h = image->h;

  center_.x = image->w / 2;
  center_.y = image->h / 2;

  SDL_FreeSurface(image);
}

void Texture::Draw(Window& window) const {
  const float rect_w_scaled_margin = (scaling_factor_ * rect_.w - rect_.w) / 2;
  const float rect_h_scaled_margin = (scaling_factor_ * rect_.h - rect_.h) / 2;

  SDL_Rect scaled_rect{static_cast<int>(rect_.x - rect_w_scaled_margin),
                       static_cast<int>(rect_.y - rect_h_scaled_margin),
                       static_cast<int>(rect_.w + 2 * rect_w_scaled_margin),
                       static_cast<int>(rect_.h + 2 * rect_h_scaled_margin)};

  SDL_SetTextureAlphaMod(texture_, transparency_);

  SDL_RenderCopyEx(window.GetRenderer().renderer,
                   texture_,
                   nullptr,
                   &scaled_rect,
                   0.0,
                   &center_,
                   flipped_ ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void Texture::SetPos(int x, int y) {
  rect_.x = x;
  rect_.y = y;
}

std::pair<int, int> Texture::GetPos() const {
  return {rect_.x, rect_.y};
}

void Texture::Flip(bool flipped) {
  flipped_ = flipped;
}

void Texture::Scale(float factor) {
  scaling_factor_ = factor;
}

void Texture::SetTransparency(std::uint8_t alpha) {
  transparency_ = alpha;
}
