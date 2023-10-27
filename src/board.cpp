#include <board.h>

#include <texture_loader.h>
#include <window.h>

#include <stdexcept>

Board::Board(TextureLoader& texture_loader, int cols, int rows)
    : texture_loader_(texture_loader), cols_(cols), rows_(rows) {
  for (int r = 0; r < rows_; ++r) {
    for (int c = 0; c < cols_; ++c) {
      fields_.emplace_back(texture_loader_.GetTexture("grass")).SetPos(c * 60, r * 60);
    }
  }
}

void Board::Draw(Window& window) {
  for (auto& field : fields_) {
    field.Draw(window);
  }
}

bool Board::Clicked(int x, int y) const {
  return x >= 0 && x <= cols_ * 60 && y >= 0 && y <= rows_ * 60;
}

void Board::ChangeTexture(int x, int y, const std::string& texture_key) {
  ChangeTexture(ToIndex(x, y), texture_key);
}

void Board::ChangeTexture(IndexType index, const std::string& texture_key) {
  auto [x, y] = TextureAt(index).GetPos();
  TextureAt(index) = texture_loader_.GetTexture(texture_key);
  TextureAt(index).SetPos(x, y);
}

IndexType Board::ToIndex(int x, int y) const {
  return y * cols_ + x;
}

void Board::InteractPress(int x, int y) {
  if (Clicked(x, y)) {
    // pos to index

  }
}

Texture& Board::TextureAt(IndexType index) {
  try {
    return fields_.at(index);
  } catch (std::out_of_range&) {
    throw std::out_of_range("Board::TextureAt: index out of range: " + std::to_string(index));
  }
}
