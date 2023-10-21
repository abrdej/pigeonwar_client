#pragma once

#include <data_packs.h>

#include <nlohmann/json.hpp>

inline void from_json(const nlohmann::json& j, EntityProperties& x) {
  x.name = j.value("name", x.name);
  x.health = j.value("health", x.health);
  x.power = j.value("power", x.power);
  x.index = j.value("index", x.index);
}
