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
    ChangeTexture(ToIndex(x, y), texture_key);
  }

  void ChangeTexture(IndexType index, const std::string& texture_key) {
    auto [x, y] = TextureAt(index).GetPos();
    TextureAt(index) = texture_loader_.GetTexture(texture_key);
    TextureAt(index).SetPos(x, y);
  }

  IndexType ToIndex(int x, int y) const {
    return y * cols_ + x;
  }

  void InteractPress(int x, int y) {
    if (Clicked(x, y)) {
      // pos to index

    }
  }

  template <typename Callback>
  void ForEachField(Callback callback) {
    for (IndexType i = 0; i < fields_.size(); ++i) {
      callback(i);
    }
  }

 private:
  Texture& TextureAt(IndexType index) {
    try {
      return fields_.at(index);
    } catch (std::out_of_range&) {
      throw std::out_of_range("Board::TextureAt: index out of range: " + std::to_string(index));
    }
  }

  TextureLoader& texture_loader_;
  int cols_;
  int rows_;
  std::vector<Texture> fields_;
};
