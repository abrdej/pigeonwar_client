#pragma once

#include <defs.h>

#include <string>
#include <unordered_set>
#include <vector>

struct LocalState {
  std::vector<IndexType> possible_movements;
  std::unordered_set<IndexType> valid_movements;
  IndexType selected_index{no_index};
  std::string actual_target_type; // TODO: consider enums??
  std::vector<std::string> button_bitmaps;
  std::vector<bool> usable;
  std::string entity_name;
};
