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

class Text {
 public:
  explicit Text(SDL_Renderer* renderer) : renderer_(renderer) {
    font_ = TTF_OpenFont((resources_directory + "verdanab.ttf").c_str(), 36);
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
    if (surface_message_) {
      SDL_FreeSurface(surface_message_);
    }
    if (message_) {
      SDL_DestroyTexture(message_);
    }
    text_ = text;
    surface_message_ = TTF_RenderText_Blended(font_, text_.c_str(), color_);
    message_ = SDL_CreateTextureFromSurface(renderer_, surface_message_);
    TTF_SizeText(font_, text.c_str(), &message_rect_.w, &message_rect_.h);
  }

  void SetPos(int x, int y) {
    message_rect_.x = x;
    message_rect_.y = y;
  }

  void SetColor(SDL_Color& color) {
    color_ = color;
  }

  void Draw(Window& window) {
    if (message_) {
      SDL_RenderCopy(window.GetRenderer(), message_, nullptr, &message_rect_);
    }
  }

 private:
  SDL_Renderer* renderer_{nullptr};
  std::string text_;
  TTF_Font* font_{nullptr};
  SDL_Color color_{0, 0, 0};
  SDL_Surface* surface_message_{nullptr};
  SDL_Texture* message_{nullptr};
  SDL_Rect message_rect_{};
};
