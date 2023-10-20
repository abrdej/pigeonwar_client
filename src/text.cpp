#include <text.h>

Text::Text(Renderer renderer, int size) : renderer_(renderer.renderer) {
  font_ = TTF_OpenFont((resources_directory + "verdanab.ttf").c_str(), size);
  if (!font_) {
    std::cout << "Failed to open font\n";
  }
}

Text::~Text() {
  SDL_FreeSurface(surface_message_);
  SDL_DestroyTexture(message_);
  TTF_CloseFont(font_);
}

void Text::SetText(const std::string& text) {
  text_ = text;
  RenderText();
}

void Text::SetPos(int x, int y) {
  message_rect_.x = x;
  message_rect_.y = y;
}

void Text::SetCenterPosX(int x, int y) {
  message_rect_.x = x - message_rect_.w / 2;
  message_rect_.y = y;
}

void Text::SetColor(const SDL_Color& color) {
  color_ = color;
  RenderText();
}

void Text::DrawBackground(bool enable) {
  background_ = enable;
}

void Text::SetBackgroundColor(const SDL_Color& color) {
  background_color_ = color;
}

void Text::Draw(Window& window) {
  if (message_) {
    if (background_) {
      Uint8 r, g, b, a;
      SDL_GetRenderDrawColor(window.GetRenderer().renderer, &r, &g, &b, &a);
      SDL_SetRenderDrawColor(window.GetRenderer().renderer,
                             background_color_.r,
                             background_color_.g,
                             background_color_.b,
                             background_color_.a);
      SDL_RenderFillRect(window.GetRenderer().renderer, &message_rect_);
      SDL_SetRenderDrawColor(window.GetRenderer().renderer, r, g, b, a);
    }
    SDL_RenderCopy(window.GetRenderer().renderer, message_, nullptr, &message_rect_);
  }
}

void Text::RenderText() {
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
