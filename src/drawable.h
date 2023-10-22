#pragma once

class Window;

struct DrawableInterface {
  virtual void Draw(Window& window) const = 0;
};
