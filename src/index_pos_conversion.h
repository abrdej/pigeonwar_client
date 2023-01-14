#pragma once

#include <cstdint>

inline void IndexToPos(std::int32_t index, int& x, int& y) {
  int col = index % 15;
  int row = index / 15;
  x = col * 60;
  y = row * 60;
}

inline void PosToIndex(int x, int y, std::int32_t& index) {
  index = (y / 60) * 15 + (x / 60);
}
