#include <message_processor.h>

#include <board.h>
#include <panel.h>

void OnEntitiesPack(EntityCollection& entity_collection) {
  // TODO: translate json_data to entities pack;

  for (int i = 0; i < 10; ++i) {
    EntityProperties entity_properties;
    // fill entity properties
    entity_collection.Add(entity_id, entity_properties);
  }

  BringEntitiesToTop();

//  var entities_pack = json_data['entities_pack'];
//
//  for (var i = 0; i < entities_pack.length; i++) {
//
//    var entity_id = entities_pack[i][0];
//    var entity_data = [];
//
//    entity_data = entities_pack[i][1];
//
//    var entity_name = entity_data[0];
//    var entity_health = entity_data[1];
//    var entity_power = entity_data[2];
//    var entity_index = entity_data[3];
//
//    entities[entity_id] =
//        new Entity(game, entity_name, entity_health, entity_power, entity_index);
//  }
//
//  bring_entities_to_top();
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
