#include <texture_loader.h>

#include <stdexcept>
#include <iostream>

#ifdef EMSCRIPTEN
static std::string resources_directory = "res/";
#else
#include <config.h>
#endif

TextureLoader::TextureLoader(Window& window) : window_(window) {}

void TextureLoader::LoadTexture(const std::string& texture_key) {
  textures_.emplace(texture_key, Texture(window_, resources_directory + texture_key + ".png"));
}

void TextureLoader::LoadTexture(const std::string& texture_key, const std::string& texture_file) {
  textures_.emplace(texture_key, Texture(window_, resources_directory + texture_file + ".png"));
}

Texture TextureLoader::GetTexture(const std::string& texture_key) const {
  try {
    return textures_.at(texture_key);
  } catch (std::out_of_range& e) {
    std::cout << "No such texture: " << texture_key << "\n";
    throw std::runtime_error("TextureLoader::GetTexture: no texture loaded for texture_key: " + texture_key);
  }
}
