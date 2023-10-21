#include <game.h>

#include <index_pos_conversion.h>
#include <json_conversions.h>

#include <nlohmann/json.hpp>

#include <chrono>
#include <iostream>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

std::unique_ptr<Text> MakeHint(Renderer renderer, const std::string& hint_message) {
  auto hint = std::make_unique<Text>(renderer, 20);
  hint->SetText(hint_message);
  hint->SetCenterPosX(15 * 60 / 2, 10 / 2 * 60);
  hint->DrawBackground(true);
  hint->SetBackgroundColor(Color{235, 235, 235});
  hint->SetBackgroundExtend(10, 10);
  return hint;
}

Game::Game()
    : entities_collection_(window_, texture_loader_) {
  texture_loader_.LoadTexture("commander");
  texture_loader_.LoadTexture("golem");
  texture_loader_.LoadTexture("grass");
  texture_loader_.LoadTexture("grass_move");
  texture_loader_.LoadTexture("grass_damage");
  texture_loader_.LoadTexture("grass_attack");
  texture_loader_.LoadTexture("grass_boost");
  texture_loader_.LoadTexture("grass_friendly");
  texture_loader_.LoadTexture("grass_selected");
  texture_loader_.LoadTexture("border");
  texture_loader_.LoadTexture("end_turn");
  texture_loader_.LoadTexture("panel_background");

  board_ = std::make_unique<Board>(texture_loader_, 15, 10);
  panel_ = std::make_unique<Panel>(window_.GetRenderer(), texture_loader_, 150, 10 * 60, 4, 5);
  panel_->OnIn([this](int button_id) {
    hint_timer_.emplace(TimerOnUpdate({[this]() {
      // Test hint
      nlohmann::json hint;
      hint["hint"] = "In the heat of battle, Golem's resilience soars, turning him into an indomitable fortress.";
      message_processor_.Process(hint);
    }, std::chrono::milliseconds(500)}), button_id);
  });
  panel_->OnOut([this](int button_id) {
    if (hint_timer_ && hint_timer_->second == button_id) {
      hint_timer_ = std::nullopt;
    }
    hint_ = nullptr;
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
    if (!hint.empty() && hint_timer_.has_value()) {
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

//  {
//    nlohmann::json entities_pack;
//
//    nlohmann::json commander;
//    commander["name"] = "commander";
//    commander["index"] = 5;
//    commander["health"] = 50;
//    nlohmann::json entity1;
//    entity1.push_back(0);
//    entity1.push_back(commander);
//
//    nlohmann::json golem;
//    golem["name"] = "golem";
//    golem["index"] = 24;
//    golem["health"] = 50;
//    golem["power"] = 25;
//    nlohmann::json entity2;
//    entity2.push_back(1);
//    entity2.push_back(golem);
//
//    entities_pack["entities_pack"].push_back(entity1);
//    entities_pack["entities_pack"].push_back(entity2);
//
//    message_processor_.Process(entities_pack);
//  }
//  {
//    // Test client id
//    nlohmann::json client_id;
//    client_id["client_id"] = 0;
//    message_processor_.Process(client_id);
//    if (client_id_ != 0) {
//      throw std::invalid_argument("Wrong client_id");
//    }
//  }
//  {
//    // Test local state
//    nlohmann::json local_state;
//    nlohmann::json state;
//    state["possible_movements"] = std::vector<std::uint32_t>{23, 25};
//    state["valid_movements"] = std::vector<std::uint32_t>{23};
//    state["selected_index"] = 24;
//    state["actual_target_type"] = "moving";
//    state["button_bitmaps"] = std::vector<std::string>{"shoot"};
//    state["usable"] = std::vector<bool>{true};
//    state["entity_name"] = "golem";
//    local_state["local_state"] = state;
//    message_processor_.Process(local_state);
//
//    if (local_state_.selected_index != 24) {
//      throw std::invalid_argument("Wrong selected_index");
//    }
//  }
//  {
//    // Test global state
//    // TODO: fix game_state to global_state
//    nlohmann::json global_state;
//    nlohmann::json state;
//
//    std::vector<std::deque<EntityIdType>> board;
//    board.resize(10 * 15);
//    board[5].push_back(0);
//    board[24].push_back(1);
//
//    state["board"] = board;
//    state["healths"] = std::unordered_map<EntityIdType, std::int32_t>{{0, 50}, {1, 50}};
//    state["entities_names"] = std::unordered_map<EntityIdType, std::string>{{0, "commander"}, {1, "golem"}};
//    state["entities_players"] = std::unordered_map<EntityIdType, PlayerIdType>{{0, 0}, {1, 0}};
//    state["entities_additional_effects"] = std::unordered_map<EntityIdType, std::vector<std::string>>{};
//    global_state["game_state"] = state;
//    message_processor_.Process(global_state);
//
//    if (global_state_.healths.at(0) != 50) {
//      throw std::invalid_argument("Wrong healths");
//    }
//  }
//  {
//    // Test remove entity
//    nlohmann::json remove_entity;
//    remove_entity["remove_entity"] = 0;
//    message_processor_.Process(remove_entity);
//  }
//  {
//    // Test move entity
//    nlohmann::json move_entity;
//    nlohmann::json data;
//    data["to_index"] = 5;
//    data["move_entity"] = 1;
//    move_entity["move_entity"] = data;
//    message_processor_.Process(move_entity);
//  }
//  {
//    // Test entity talk
//    {
//      nlohmann::json entity_talk;
//      nlohmann::json data;
//      data["entity_talk"] = 24;
//      data["text"] = "Hello my world, I come to destroy you!";
//      data["hold_ms"] = 2000;
//      entity_talk["entity_talk"] = data;
//      message_processor_.Process(entity_talk);
//    }
//    {
//      nlohmann::json entity_talk;
//      nlohmann::json data;
//      data["entity_talk"] = 24;
//      data["text"] = "This is the end!";
//      data["hold_ms"] = 1000;
//      entity_talk["entity_talk"] = data;
//      message_processor_.Process(entity_talk);
//    }
//  }

  window_.OnMousePressed([this](int x, int y) {
    if (board_->Clicked(x, y)) {
      std::int32_t index = PosToIndex(x, y);
      std::cout << "Clicked board on index: " << index << "\n";

      // TODO: send message to server

    } else if (panel_->Clicked(x, y)) {
      std::cout << "Panel clicked\n";

      // TODO: send message to server
    }

    panel_->InteractPress(x, y);
  });

  window_.OnMouseMove([this](int x, int y) {
    panel_->InteractMove(x, y);
  });

//  animation_ = std::make_unique<MoveAnimation>();
//  animation_->to_index = 17;
//  std::cout << "handle\n";
//  animation_->Handle(entities_collection_.Get(0));
//  std::cout << "handle2\n";
//
//  scale_animation_ = std::make_unique<ScaleAnimation>();
//  scale_animation_->Handle(entities_collection_.Get(1));
//
//  transparency_animation_ = std::make_unique<TransparencyAnimation>();
//  transparency_animation_->Handle(entities_collection_.Get(1));

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
  last_update_ = std::chrono::system_clock::now();

#ifdef EMSCRIPTEN
  emscripten_set_main_loop_arg(LoopHandler, this, 0, 1);
#else
  while (window_.IsOpen()) {
    ExecuteLoop();
  }
#endif
}

void Game::ExecuteLoop() {
  client_.Update(1);
  window_.ProcessEvents();

  auto now = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update_);
  time_since_update_ += duration;
  while (time_since_update_ > time_per_frame_) {
    last_update_ = now;
    time_since_update_ -= time_per_frame_;
    window_.ProcessEvents();
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
  if (animation_ && animation_->Update(delta_time)) {
    animation_ = nullptr;
  }
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
  // TODO: check if this will be required
//    BringEntitiesToTop();
}

void Game::OnLocalState(const MessageType& message) {
  local_state_ = message;
  UpdateBoardState();
  UpdatePanelState();
}

void Game::OnGlobalState(const MessageType& message) {
  global_state_ = message;
  // TODO: do we need this
//    for (const auto [entity_id, player_id] : global_state_.entities_players) {
//      entities_collection_.Get(entity_id).SetPlayer(player_id);
//    }
}

void Game::OnAnimation(const MessageType& message) {

}

void Game::UpdateBoardState() {
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
