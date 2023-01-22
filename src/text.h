#pragma once

#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <window.h>

#ifdef EMSCRIPTEN
static std::string resources_directory = "res/";
#else
#include <config.h>
#endif

// TODO: use smart pointers to free resources
// TODO: we need to load the font only once per game

class Text {
 public:
  explicit Text(SDL_Renderer* renderer, int size) : renderer_(renderer) {
    font_ = TTF_OpenFont((resources_directory + "verdanab.ttf").c_str(), size);
    if (!font_) {
      std::cout << "Failed to open font\n";
    }
  }

  ~Text() {
    SDL_FreeSurface(surface_message_);
    SDL_DestroyTexture(message_);
    TTF_CloseFont(font_);
  }

  void SetText(const std::string& text) {
    text_ = text;
    RenderText();
  }

  void SetPos(int x, int y) {
    message_rect_.x = x;
    message_rect_.y = y;
  }

  void SetCenterPosX(int x, int y) {
    message_rect_.x = x - message_rect_.w / 2;
    message_rect_.y = y;
  }

  void SetColor(const SDL_Color& color) {
    color_ = color;
    RenderText();
  }

  void Draw(Window& window) {
    if (message_) {
      SDL_RenderCopy(window.GetRenderer(), message_, nullptr, &message_rect_);
    }
  }

 private:
  void RenderText() {
    if (surface_message_) {
      SDL_FreeSurface(surface_message_);
    }
    if (message_) {
      SDL_DestroyTexture(message_);
    }
    surface_message_ = TTF_RenderText_Blended(font_, text_.c_str(), color_);
    message_ = SDL_CreateTextureFromSurface(renderer_, surface_message_);
    TTF_SizeText(font_, text_.c_str(), &message_rect_.w, &message_rect_.h);
  }

  SDL_Renderer* renderer_{nullptr};
  std::string text_;
  TTF_Font* font_{nullptr};
  SDL_Color color_{0, 0, 0};
  SDL_Surface* surface_message_{nullptr};
  SDL_Texture* message_{nullptr};
  SDL_Rect message_rect_{};
};
