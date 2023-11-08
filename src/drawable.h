#pragma once

class Window;

struct DrawableInterface {
  virtual ~DrawableInterface() = default;
  virtual void Draw(Window& window) const = 0;
};
