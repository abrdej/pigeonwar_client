#pragma once

#include <defs.h>

#include <string>

struct EntityProperties {
  std::string name;
  std::int32_t health{no_health};
  std::int32_t power{no_power};
  IndexType index{no_index};
};
