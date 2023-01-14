#include <window.h>

#include <iostream>

#include <SDL2/SDL_image.h>

Window::Window() {
  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

  if (SDL_CreateWindowAndRenderer(960, 640, 0, &window_, &renderer_) != 0) {
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
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_KEYDOWN:
      std::cout << "Key press detected\n";
      break;

    case SDL_KEYUP:
      std::cout << "Key release detected\n";
      break;

    case SDL_QUIT:
      std::cout << "Quit\n";
      is_open_ = false;
      break;

    default:
      break;
  }

  switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
      std::cout << "ESCAPE\n";
      is_open_ = false;
      break;
  }
}

void Window::Clear() {
  SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
  SDL_RenderClear(renderer_);
}

void Window::Display() {
  SDL_RenderPresent(renderer_);
}
