#include <message_processor.h>

#include <board.h>
#include <panel.h>

//void OnLocalState(const LocalState& local_state, Board& board, Panel& panel, Entities& entities) {
//  // Update the board
//  if (local_state.actual_target_type != "non") {
//    auto is_valid_movement = [&valid_movements = local_state.valid_movements](std::int32_t index) {
//      return std::find(std::begin(valid_movements), std::end(valid_movements), index)
//          != std::end(valid_movements);
//    };
//
//    for (const auto possible_movement : local_state.possible_movements) {
//      auto [x, y] = IndexToPos(possible_movement);
//      if (local_state.actual_target_type == "moving") {
//        board.ChangeTexture(x, y, "grass_move");
//
//      } else if (local_state.actual_target_type == "enemy") {
//        if (is_valid_movement(possible_movement)) {
//          board.ChangeTexture(x, y, "grass_damage");
//        } else {
//          board.ChangeTexture(x, y, "grass_attack");
//        }
//
//      } else if (local_state.actual_target_type == "friendly") {
//        if (is_valid_movement(possible_movement)) {
//          board.ChangeTexture(x, y, "grass_boost");
//        } else {
//          board.ChangeTexture(x, y, "grass_friendly");
//        }
//      } else {
//        board.ChangeTexture(x, y, "grass_move");
//      }
//    }
//  }
//
//  if (local_state.selected_index != no_selected_index) {
//    auto [x, y] = IndexToPos(local_state.selected_index);
//    board.ChangeTexture(x, y, "grass_selected");
//  }
//
//  // Update for entity
//  if (local_state.selected_index == no_selected_index) {
//    return;
//  }
//
//  // TODO: logging
////  console.log("selected_index: " + selected_index);
//
//  auto it = entities.find(local_state.selected_index);
//  if (it != std::end(entities)) {
//    panel.UpdateForEntity(it->second);
//  }
//}

void MessageProcessor::OnMessage(const std::string& message_type, CallbackType callback) {
  callbacks_.emplace(message_type, callback);
}

void MessageProcessor::Process(const MessageDataType& message) {
  try {
    for (const auto& [message_type, callback] : callbacks_) {
      if (message.contains(message_type)) {
        callbacks_.at(message_type)(message[message_type]);
        break;
      }
    }
  } catch (std::out_of_range& e) {
    throw std::out_of_range(
        "MessageProcessor::Process: no callback registered for this message type: " + std::string(e.what()));
  }
}
