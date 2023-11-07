#include <game.h>

#include <index_pos_conversion.h>
#include <json_conversions.h>
#include <load_resources.h>

#include <nlohmann/json.hpp>

#include <chrono>
#include <iostream>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

std::unique_ptr<Text> MakeHint(Renderer renderer, const std::string& hint_message) {
  auto hint = std::make_unique<Text>(renderer, 20);
  hint->SetText(hint_message);
  hint->SetAnchor(0.5, 0.0);
  hint->SetPos(15 * 60 / 2, 10 / 2 * 60);
  hint->DrawBackground(true);
  hint->SetBackgroundColor(Color{235, 235, 235});
  hint->SetBackgroundExtend(10, 10);
  return hint;
}

Game::Game()
    : entities_collection_(window_, texture_loader_) {
  LoadResources(texture_loader_);

  board_ = std::make_unique<Board>(texture_loader_, 15, 10);
  panel_ = std::make_unique<Panel>(window_.GetRenderer(), texture_loader_, 150, 10 * 60, 4, 5);
  panel_->OnIn([this](auto button_id) { return OnGetHint(button_id); });
  panel_->OnOut([this](ButtonId button_id) {
    if (hint_timer_ && hint_timer_->second == button_id) {
      hint_timer_ = std::nullopt;
      hint_requested_ = false;
    }
    hint_ = nullptr;
  });
  panel_->OnClicked([this](ButtonId button_id) {
    nlohmann::json on_button;
    on_button["on_button"]["client_id"] = client_id_;

    std::visit(Overloaded{
        [&on_button](AbilityButtonId x) {
          on_button["on_button"]["button"] = x.index;
        },
        [&on_button](EndTurnButtonId x) {
          on_button["on_button"]["button"] = 5;
        },
        [](EffectButtonId x) {},
        [](EntityButtonId x) {}
    }, button_id);

    SendMessage(on_button.dump());
  });

  message_processor_.OnMessage(entities_pack_message, &Game::OnEntityPack, this);
  message_processor_.OnMessage(client_id_message, [this](const auto& message) {
    client_id_ = message;
  });
  message_processor_.OnMessage(local_state_message, &Game::OnLocalState, this);
  message_processor_.OnMessage(global_state_message, &Game::OnGlobalState, this);
  message_processor_.OnMessage(animation_message, &Game::OnAnimation, this);
  message_processor_.OnMessage(remove_entity_message, [this](const auto& message) {
    EntityIdType entity_id = message;
    entities_collection_.Remove(entity_id);
  });
  message_processor_.OnMessage(move_entity_message, [this](const auto& message) {
    IndexType to_index = message["to_index"];
    EntityIdType entity_id = message["move_entity"];
    entities_collection_.Get(entity_id).SetIndex(to_index);
  });
  message_processor_.OnMessage(hint_message, [this](const auto& message) {
    std::string hint = message;
    std::cout << "Got hint message: " << hint << "\n";
    if (!hint.empty() && hint_requested_) {
      std::cout << "Creating a hint\n";
      hint_ = MakeHint(window_.GetRenderer(), hint);
    }
  });
  message_processor_.OnMessage(entity_talk_message, [this](const auto& message) {
    IndexType entity_index = message["entity_talk"];
    std::string text = message["text"];
    int hold_ms = message.value("hold_ms", 500);
    local_state_.selected_index = entity_index;
    UpdateBoardState();
    UpdatePanelState();
    EnqueueTalk(text, std::chrono::milliseconds(hold_ms));
  });
  message_processor_.OnMessage(game_ready_message, [this](const auto&) {
    game_ready_ = true;
  });
  message_processor_.OnMessage(defeat_message, [this](const auto&) {
    std::cout << "Game defeat!!!\n";
  });
  message_processor_.OnMessage(victory_message, [this](const auto&) {
    std::cout << "victory_message!!!\n";
  });

  window_.OnMousePressed([this](int x, int y) {
    if (board_->Clicked(x, y)) {
      std::int32_t index = ScreenPosToIndex(x, y);
      std::cout << "Clicked board on index: " << index << "\n";

      nlohmann::json on_board;
      on_board["on_board"]["client_id"] = client_id_;
      on_board["on_board"]["index"] = index;
      SendMessage(on_board.dump());

    } else if (panel_->Clicked(x, y)) {
      std::cout << "Panel clicked\n";

      // TODO: send message to server
    }
    panel_->InteractPress(x, y);
  });

  window_.OnMouseMove([this](int x, int y) {
    panel_->InteractMove(x, y);
  });

  client_.OnMessage([this](const std::string& message) {
    ProcessMessage(message);
  });

  // TODO: configure the address
  if (!client_.Connect("127.0.0.1", 60001)) {
    std::cout << "Failed to connect\n";
  }
}

#ifdef EMSCRIPTEN
void LoopHandler(void* game) {
  Game* this_game = reinterpret_cast<Game*>(game);
  this_game->ExecuteLoop();
}
#endif

void Game::Run() {
  last_update_ = std::chrono::steady_clock::now();

#ifdef EMSCRIPTEN
  emscripten_set_main_loop_arg(LoopHandler, this, 0, 1);
#else
  while (window_.IsOpen()) {
    ExecuteLoop();
  }
#endif
}

void Game::ExecuteLoop() {
  if (animations_.empty()) {
    client_.Update(1);
    window_.ProcessEvents();
  }
  auto now = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update_);
  time_since_update_ += duration;
  while (time_since_update_ > time_per_frame_) {
    last_update_ = now;
    time_since_update_ -= time_per_frame_;
    if (animations_.empty()) {
      window_.ProcessEvents();
    }
    Update(time_per_frame_);
  }
  Render();
}

void Game::Render() {
  window_.Clear();
  board_->Draw(window_);
  panel_->Draw(window_);
  entities_collection_.Draw(window_);
  if (hint_) {
    hint_->Draw(window_);
  }
  if (talk_) {
    talk_->Draw(window_);
  }
  for (const auto& animations : animations_) {
    animations->Draw(window_);
  }
  window_.Display();
}

void Game::Update(std::chrono::milliseconds delta_time) {
  if (hint_timer_) {
    if (hint_timer_->first.Update()) {
      hint_timer_ = std::nullopt;
    }
  }
  if (talk_timer_) {
    if (talk_timer_->Update()) {
      talk_timer_ = std::nullopt;
    }
  }
  animations_.erase(std::remove_if(std::begin(animations_),
                                   std::end(animations_),
                                   [delta_time](auto& x) {
                                     return x->Update(delta_time);
                                   }), std::end(animations_));

  if (scale_animation_ && scale_animation_->Update(delta_time)) {
    scale_animation_ = nullptr;
  }
  if (transparency_animation_ && transparency_animation_->Update(delta_time)) {
    transparency_animation_ = nullptr;
  }
  ProcessCallbacks();
}

void Game::OnEntityPack(const MessageType& message) {
  std::cout << "Got entities message pack\n";
  EntityPack entity_pack = message;
  for (const auto& [entity_id, entity_data] : entity_pack) {
    entities_collection_.Add(entity_id, entity_data);
  }
  entities_collection_.SortEntitiesOrder();
}

void Game::OnLocalState(const MessageType& message) {
  local_state_ = message;
  UpdateBoardState();
  UpdatePanelState();
}

void Game::OnGlobalState(const MessageType& message) {
  global_state_ = message;

  for (auto [entity_id, player_id] : global_state_.entities_players) {
    entities_collection_.Get(entity_id).SetPlayer(player_id);
  }
}

void Game::OnAnimation(const MessageType& message) {
  std::string animation = message[0];
  if (animation == "move") {
    ClearSelectedIndex();

    EntityIdType entity_id = message[1];
    IndexType index = message[2];
    animations_.emplace_back(
        std::make_unique<MoveAnimation>(entities_collection_.Get(entity_id), index));

  } else if (animation == "change_health") {
    EntityIdType entity_id = message[1];
    HealthType change_amount = message[2];
    animations_.emplace_back(
        std::make_unique<ChangeHealthAnimation>(window_.GetRenderer(),
                                                entities_collection_.Get(entity_id), change_amount));
  } else if (animation == "shoot") {
    IndexType source_index = message[1];
    IndexType target_index = message[2];
    animations_.emplace_back(
        std::make_unique<ShotAnimation>(texture_loader_, source_index, target_index));
  }
}

void Game::UpdateBoardState() {
  board_->ForEachField([this](IndexType index) {
    board_->ChangeTexture(index, "grass");
  });
  if (local_state_.actual_target_type != "non") {
    for (const auto possible_movement : local_state_.possible_movements) {
      if (local_state_.actual_target_type == "moving") {
        board_->ChangeTexture(possible_movement, "grass_move");

      } else if (local_state_.actual_target_type == "enemy") {
        if (local_state_.valid_movements.count(possible_movement)) {
          board_->ChangeTexture(possible_movement, "grass_damage");
        } else {
          board_->ChangeTexture(possible_movement, "grass_attack");
        }

      } else if (local_state_.actual_target_type == "friendly") {
        if (local_state_.valid_movements.count(possible_movement)) {
          board_->ChangeTexture(possible_movement, "grass_boost");
        } else {
          board_->ChangeTexture(possible_movement, "grass_friendly");
        }
      }
    }
  }
  if (local_state_.selected_index != no_index) {
    board_->ChangeTexture(local_state_.selected_index, "grass_selected");
  }
}

void Game::UpdatePanelState() {
  if (local_state_.selected_index == no_index) {
    return;
  }
  auto entity_properties = entities_collection_.EntityPropertiesForIndex(local_state_.selected_index);
  if (entity_properties) {
    panel_->SetCurrentEntity(*entity_properties);
  }
  panel_->SetLocalState(local_state_);
}

void Game::EnqueueTalk(const std::string& text, std::chrono::milliseconds hold) {
  if (talk_) {
    text_to_talk_.emplace(text, hold);
    return;
  }
  talk_ = MakeHint(window_.GetRenderer(), text);
  talk_timer_.emplace([this, text]() {
    talk_ = nullptr;
    if (!text_to_talk_.empty()) {
      EnqueueCallback([this]() {
        auto [next_text, next_hold] = text_to_talk_.front();
        text_to_talk_.pop();
        EnqueueTalk(next_text, next_hold);
      });
    }
  }, std::chrono::milliseconds(hold));
}

void Game::EnqueueCallback(std::function<void()> callback) {
  callbacks_.push(std::move(callback));
}

void Game::ProcessCallbacks() {
  while (!callbacks_.empty()) {
    auto callback = callbacks_.front();
    callbacks_.pop();
    callback();
  }
}

void Game::ProcessMessage(const std::string& str_message) {
  auto message = nlohmann::json::parse(str_message);
  message_processor_.Process(message);
}

void Game::SendMessage(const std::string& message) {
  client_.Send(message);
}

void Game::OnGetHint(ButtonId button_id) {
  if (Is<EndTurnButtonId>(button_id)) {
    return;
  }
  auto make_message = [this, button_id]() {
    nlohmann::json get_hint;
    std::visit(Overloaded{
        [this, &get_hint](AbilityButtonId x) {
          get_hint["get_button_description"]["client_id"] = client_id_;
          get_hint["get_button_description"]["button"] = x.index;
        },
        [this, &get_hint](EffectButtonId x) {
          get_hint["get_effect_description"]["client_id"] = client_id_;
          get_hint["get_effect_description"]["effect"] = x.index;
        },
        [this, &get_hint](EntityButtonId x) {
          get_hint["get_entity_description"]["client_id"] = client_id_;
        },
        [&get_hint](EndTurnButtonId x) {
          get_hint["on_button"]["button"] = 5;
        }
    }, button_id);
    return get_hint.dump();
  };
  hint_timer_.emplace(TimerOnUpdate({[this, make_message]() {
    hint_requested_ = true;
    SendMessage(make_message());
  }, std::chrono::milliseconds(500)}), button_id);
}

void Game::ClearSelectedIndex() {
  local_state_.selected_index = no_index;
  UpdateBoardState();
}
