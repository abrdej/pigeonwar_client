#pragma once

#include <vector>

#include <window.h>
#include <entity.h>
#include <texture_loader.h>

class Board {
 public:
  Board(TextureLoader& texture_loader, int cols, int rows) : cols_(cols), rows_(rows) {
    for (int r = 0; r < rows_; ++r) {
      for (int c = 0; c < cols_; ++c) {
        fields_.push_back(texture_loader.GetTexture("grass"));
        fields_.back().SetPos(c * 60, r * 60);
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

 private:
  int cols_;
  int rows_;
  std::vector<Texture> fields_;
};
