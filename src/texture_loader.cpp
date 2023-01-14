#include <texture_loader.h>

#include <stdexcept>

TextureLoader::TextureLoader(Window& window) : window_(window) {
}

void TextureLoader::LoadTexture(const std::string& texture_key, const std::string& texture_file) {
  textures_.emplace(texture_key, Texture(window_, texture_file));
}

Texture TextureLoader::GetTexture(const std::string& texture_key) {
  try {
    return textures_.at(texture_key);
  } catch (std::out_of_range& e) {
    throw std::runtime_error("TextureLoader::GetTexture: no texture loaded for texture_key: " + texture_key);
  }
}
