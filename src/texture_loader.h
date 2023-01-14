#pragma once

#include <texture.h>

#include <string>
#include <unordered_map>

class Window;

class TextureLoader {
 public:
  TextureLoader(Window& window);

  Texture GetTexture(const std::string& texture_key);

  void LoadTexture(const std::string& texture_key, const std::string& texture_file);

 private:
  Window& window_;
  std::unordered_map<std::string, Texture> textures_;
};
