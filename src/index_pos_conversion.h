#pragma once

#include <defs.h>

#include <cstdint>

inline std::pair<int, int> IndexToPos(IndexType index) {
  int col = static_cast<int>(index) % 15;
  int row = static_cast<int>(index) / 15u;
  return {col * 60, row * 60};
}

inline IndexType PosToIndex(int x, int y) {
  return (y / 60) * 15 + (x / 60);
}
