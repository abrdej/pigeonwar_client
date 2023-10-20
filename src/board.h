#pragma once

#include <vector>

#include <window.h>
#include <entity.h>
#include <texture_loader.h>

class Board {
 public:
  Board(TextureLoader& texture_loader, int cols, int rows)
      : texture_loader_(texture_loader), cols_(cols), rows_(rows) {
    for (int r = 0; r < rows_; ++r) {
      for (int c = 0; c < cols_; ++c) {
        fields_.emplace_back(texture_loader_.GetTexture("grass")).SetPos(c * 60, r * 60);
      }
    }
  }

  void Draw(Window& window) {
    for (auto& field : fields_) {
      field.Draw(window);
    }
  }

  // TODO: decide if we want to test it here?
  [[nodiscard]] bool Clicked(int x, int y) const {
    return x >= 0 && x <= cols_ * 60 && y >= 0 && y <= rows_ * 60;
  }

  void ChangeTexture(int x, int y, const std::string& texture_key) {
    TextureAt(x, y) = texture_loader_.GetTexture(texture_key);
  }

 private:
  Texture& TextureAt(int x, int y) {
    try {
      return fields_.at(y * cols_ + x);
    } catch (std::out_of_range&) {
      throw std::out_of_range("Board::TextureAt: x, y out of range: " + std::to_string(x) + ", " + std::to_string(y));
    }
  }

  TextureLoader& texture_loader_;
  int cols_;
  int rows_;
  std::vector<Texture> fields_;
};
