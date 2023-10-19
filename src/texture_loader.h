#pragma once

#include <texture.h>

#include <string>
#include <unordered_map>

class Window;

class TextureLoader {
 public:
  explicit TextureLoader(Window& window);

  Texture GetTexture(const std::string& texture_key);

  void LoadTexture(const std::string& texture_key);

 private:
  Window& window_;
  std::unordered_map<std::string, Texture> textures_;
};
