#pragma once

#include <data_packs.h>
#include <local_state.h>

#include <nlohmann/json.hpp>

inline void from_json(const nlohmann::json& j, EntityProperties& x) {
  x.name = j.value("name", x.name);
  x.health = j.value("health", x.health);
  x.power = j.value("power", x.power);
  x.index = j.value("index", x.index);
}

inline void from_json(const nlohmann::json& j, LocalState& x) {
  x.possible_movements = j.at("possible_movements").get<decltype(x.possible_movements)>();
  x.valid_movements = j.at("valid_movements").get<decltype(x.valid_movements)>();
  x.selected_index = j.at("selected_index");
  x.actual_target_type = j.at("actual_target_type");
  x.button_bitmaps = j.at("button_bitmaps");
  x.usable = j.at("usable").get<decltype(x.usable)>();
  x.entity_name = j.at("entity_name");
}
