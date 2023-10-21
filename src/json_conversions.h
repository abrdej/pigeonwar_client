#pragma once

#include <data_packs.h>
#include <game_state.h>

#include <nlohmann/json.hpp>

inline void from_json(const nlohmann::json& j, EntityProperties& x) {
  x.name = j.value("name", x.name);
  x.health = j.value("health", x.health);
  x.power = j.value("power", x.power);
  x.index = j.value("index", x.index);
}

inline void from_json(const nlohmann::json& j, LocalGameState& x) {
  x.possible_movements = j.at("possible_movements").get<decltype(x.possible_movements)>();
  x.valid_movements = j.at("valid_movements").get<decltype(x.valid_movements)>();
  x.selected_index = j.at("selected_index");
  x.actual_target_type = j.at("actual_target_type");
  x.button_bitmaps = j.at("button_bitmaps");
  x.usable = j.at("usable").get<decltype(x.usable)>();
  x.entity_name = j.at("entity_name");
}

inline void from_json(const nlohmann::json& j, GlobalGameState& x) {
  x.board = j.at("board").get<decltype(x.board)>();
  x.healths = j.at("healths").get<decltype(x.healths)>();
  x.entities_names = j.at("entities_names");
  x.entities_players = j.at("entities_players");
  x.entities_additional_effects = j.at("entities_additional_effects");
}
