#include <texture.h>

#include <string>
#include <stdexcept>

#include <SDL2/SDL_image.h>

Texture::Texture(Window& window, const std::string &texture_file) {
  SDL_Surface *image = IMG_Load(texture_file.c_str());
  if (!image) {
    throw std::runtime_error("Texture: " + std::string(IMG_GetError()));
  }
  texture_ = SDL_CreateTextureFromSurface(window.GetRenderer(), image);
  rect_.x = 0;
  rect_.y = 0;
  rect_.w = image->w;
  rect_.h = image->h;

  SDL_FreeSurface(image);
}

void Texture::Draw(Window& window) {
  SDL_RenderCopy(window.GetRenderer(), texture_, nullptr, &rect_);
}

void Texture::SetPos(int x, int y) {
  rect_.x = x;
  rect_.y = y;
}

void Texture::GetPos(int& x, int& y) {
  x = rect_.x;
  y = rect_.y;
}
