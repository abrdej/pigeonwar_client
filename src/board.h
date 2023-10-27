#pragma once

#include <defs.h>
#include <texture.h>

#include <vector>

enum class ButtonsKeys : std::int32_t {
  Entity,
  Ability1,
  Ability2,
  Ability3,
  Ability4,
  EndTurn
};

class Window;
class TextureLoader;

class Board {
 public:
  Board(TextureLoader& texture_loader, int cols, int rows);

  void Draw(Window& window);

  // TODO: decide if we want to test it here?
  [[nodiscard]] bool Clicked(int x, int y) const;

  void ChangeTexture(int x, int y, const std::string& texture_key);

  void ChangeTexture(IndexType index, const std::string& texture_key);

  IndexType ToIndex(int x, int y) const;

  void InteractPress(int x, int y);

  template <typename Callback>
  void ForEachField(Callback callback) {
    for (IndexType i = 0; i < fields_.size(); ++i) {
      callback(i);
    }
  }

 private:
  Texture& TextureAt(IndexType index);

  TextureLoader& texture_loader_;
  int cols_;
  int rows_;
  std::vector<Texture> fields_;
};
