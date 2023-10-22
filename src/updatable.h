#pragma once

#include <chrono>

struct UpdatableInterface {
  virtual bool Update(std::chrono::milliseconds delta_time) = 0;
};
