#include <message_processor.h>

#include <board.h>
#include <panel.h>

void OnEntitiesPack(const EntityPack& entity_pack, EntityCollection& entity_collection) {
  for (const auto& [entity_id, entity_properties] : entity_pack) {
    entity_collection.Add(entity_id, entity_properties);
  }
  BringEntitiesToTop();
}

void OnLocalState(const LocalState& local_state, Board& board, Panel& panel, Entities& entities) {
  // Update the board
  if (local_state.actual_target_type != "non") {
    auto is_valid_movement = [&valid_movements = local_state.valid_movements](std::int32_t index) {
      return std::find(std::begin(valid_movements), std::end(valid_movements), index)
          != std::end(valid_movements);
    };

    for (const auto possible_movement : local_state.possible_movements) {
      auto [x, y] = IndexToPos(possible_movement);
      if (local_state.actual_target_type == "moving") {
        board.ChangeTexture(x, y, "grass_move");

      } else if (local_state.actual_target_type == "enemy") {
        if (is_valid_movement(possible_movement)) {
          board.ChangeTexture(x, y, "grass_damage");
        } else {
          board.ChangeTexture(x, y, "grass_attack");
        }

      } else if (local_state.actual_target_type == "friendly") {
        if (is_valid_movement(possible_movement)) {
          board.ChangeTexture(x, y, "grass_boost");
        } else {
          board.ChangeTexture(x, y, "grass_friendly");
        }
      } else {
        board.ChangeTexture(x, y, "grass_move");
      }
    }
  }

  if (local_state.selected_index != no_selected_index) {
    auto [x, y] = IndexToPos(local_state.selected_index);
    board.ChangeTexture(x, y, "grass_selected");
  }

  // Update for entity
  if (local_state.selected_index == no_selected_index) {
    return;
  }

  // TODO: logging
//  console.log("selected_index: " + selected_index);

  auto it = entities.find(local_state.selected_index);
  if (it != std::end(entities)) {
    panel.UpdateForEntity(it->second);
  }
}
