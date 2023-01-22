#include <window.h>

#include <iostream>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

Window::Window() {
  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
  TTF_Init();

  if (SDL_CreateWindowAndRenderer(900, 700, 0, &window_, &renderer_) != 0) {
    throw std::runtime_error("Failed to create window and renderer");
  }
}

Window::~Window() {
  IMG_Quit();
  SDL_Quit();
}

bool Window::IsOpen() const {
  return is_open_;
}

void Window::ProcessEvents() {
  SDL_Event event;
  if (SDL_PollEvent(&event) == 0) {
    return;
  }

  switch (event.type) {
    case SDL_KEYDOWN:
      std::cout << "Key press detected\n";

      switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          std::cout << "ESCAPE\n";
          is_open_ = false;
          break;
      }

      break;

    case SDL_KEYUP:
      std::cout << "Key release detected\n";
      break;

    case SDL_QUIT:
      std::cout << "Quit\n";
      is_open_ = false;
      break;

    case SDL_MOUSEBUTTONDOWN:
      MousePress(event.button);
      break;

    case SDL_MOUSEBUTTONUP:

      break;

    case SDL_MOUSEMOTION:
      MouseMove(event.motion);
      break;

    default:
      break;
  }
}

void Window::Clear() {
  SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
  SDL_RenderClear(renderer_);
}

void Window::Display() {
  SDL_RenderPresent(renderer_);
}

void Window::MousePress(SDL_MouseButtonEvent& event) {
  if (event.button == SDL_BUTTON_LEFT) {
    std::cout << "Left press on : " << event.x << ", " << event.y << "\n";

    on_mouse_press_(event.x, event.y);
  }
}

void Window::MouseMove(SDL_MouseMotionEvent& event) {
  on_mouse_move_(event.x, event.y);
}
