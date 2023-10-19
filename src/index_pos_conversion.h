#pragma once

#include <cstdint>

inline std::pair<int, int> IndexToPos(std::int32_t index) {
  int col = index % 15;
  int row = index / 15;
  return {col * 60, row * 60};
}

inline std::int32_t PosToIndex(int x, int y) {
  return (y / 60) * 15 + (x / 60);
}
