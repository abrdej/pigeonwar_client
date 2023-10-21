#pragma once

#include <defs.h>

#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct LocalGameState {
  std::vector<IndexType> possible_movements;
  std::unordered_set<IndexType> valid_movements;
  IndexType selected_index{no_index};
  std::string actual_target_type; // TODO: consider enums??
  std::vector<std::string> button_bitmaps;
  std::vector<bool> usable;
  std::string entity_name;
};

struct GlobalGameState {
  std::vector<std::deque<EntityIdType>> board;
  std::unordered_map<EntityIdType, std::int32_t> healths;
  std::unordered_map<EntityIdType, std::string> entities_names;
  std::unordered_map<EntityIdType, PlayerIdType> entities_players;
  std::unordered_map<EntityIdType, std::vector<std::string>> entities_additional_effects;
};
