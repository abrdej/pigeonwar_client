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
  message_rect_.x = x - anchor_.x * message_rect_.w;
  message_rect_.y = y - anchor_.y * message_rect_.h;
  UpdateBackgroundRect();
}

void Text::SetAnchor(float x, float y) {
  anchor_.x = x;
  anchor_.y = y;
  message_rect_.x = x - anchor_.x * message_rect_.w;
  message_rect_.y = y - anchor_.y * message_rect_.h;
}

void Text::SetColor(const Color& color) {
  color_ = ToSDL(color);
  RenderText();
}

void Text::DrawBackground(bool enable) {
  background_ = enable;
}

void Text::SetBackgroundColor(const Color& color) {
  background_color_ = ToSDL(color);
}

void Text::SetBackgroundExtend(int x, int y) {
  background_extend_x_ = x;
  background_extend_y_ = y;
  UpdateBackgroundRect();
}

std::pair<int, int> Text::GetPos() const {
  return {message_rect_.x + anchor_.x * message_rect_.w, message_rect_.y + anchor_.y * message_rect_.h};
}

void Text::Draw(Window& window) {
  if (text_ == "dfsdfsdfsd" && !message_) {
    std::cout << "message_ is null\n";
  }
  if (message_) {
    if (background_) {
      Uint8 r, g, b, a;
      SDL_GetRenderDrawColor(window.GetRenderer().renderer, &r, &g, &b, &a);
      SDL_SetRenderDrawColor(window.GetRenderer().renderer,
                             background_color_.r,
                             background_color_.g,
                             background_color_.b,
                             background_color_.a);
      SDL_RenderFillRect(window.GetRenderer().renderer, &background_rect_);
      SDL_SetRenderDrawColor(window.GetRenderer().renderer, r, g, b, a);
    }
    if (text_ == "dfsdfsdfsd") {
      std::cout << "Render copy\n";
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
//  if (text_.empty()) {
//    return;
//  }

  if (text_ == "dfsdfsdfsd") {
    std::cout << "RenderText: " << text_ << "\n";
  }


  surface_message_ = TTF_RenderText_Blended_Wrapped(font_, text_.c_str(), color_, 600);

  if (text_ == "dfsdfsdfsd" && !surface_message_) {
    std::cout << "Failed to create a surface\n";
  }

  message_ = SDL_CreateTextureFromSurface(renderer_, surface_message_);

  if (text_ == "dfsdfsdfsd" && !message_) {
    std::cout << "Failed to create a message_\n";
  }

  message_rect_.w = surface_message_->w;
  message_rect_.h = surface_message_->h;
  UpdateBackgroundRect();
}

void Text::UpdateBackgroundRect() {
  background_rect_ = message_rect_;
  background_rect_.x -= background_extend_x_;
  background_rect_.y -= background_extend_y_;
  background_rect_.w += 2 * background_extend_x_;
  background_rect_.h += 2 * background_extend_y_;
}
