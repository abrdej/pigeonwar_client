#pragma once

#include <renderer.h>

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
  explicit Text(Renderer renderer, int size);

  ~Text();

  void SetText(const std::string& text);

  void SetPos(int x, int y);

  void SetCenterPosX(int x, int y);

  void SetColor(const SDL_Color& color);

  void DrawBackground(bool enable);

  void SetBackgroundColor(const SDL_Color& color);

  void SetBackgroundExtend(int x, int y);

  void Draw(Window& window);

 private:
  void RenderText();
  void UpdateBackgroundRect();

  SDL_Renderer* renderer_{nullptr};
  std::string text_;
  TTF_Font* font_{nullptr};
  SDL_Color color_{0, 0, 0};
  SDL_Surface* surface_message_{nullptr};
  SDL_Texture* message_{nullptr};
  SDL_Rect message_rect_{};
  SDL_Color background_color_{0, 0, 0};
  bool background_{false};
  SDL_Rect background_rect_;
  int background_extend_x_{0};
  int background_extend_y_{0};
};
