#pragma once

#include <chrono>

struct UpdatableInterface {
  virtual ~UpdatableInterface() = default;
  virtual bool Update(std::chrono::milliseconds delta_time) = 0;
};
