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
  texture_loader_.LoadTexture("border");
  texture_loader_.LoadTexture("end_turn");
  texture_loader_.LoadTexture("panel_background");

  board_ = std::make_unique<Board>(texture_loader_, 15, 10);
  panel_ = std::make_unique<Panel>(window_.GetRenderer(), texture_loader_, 150, 10 * 60, 4, 5);

  message_processor_.OnMessage(entities_pack_message, [this](const nlohmann::json& message) {
    EntityPack entity_pack = message;
    for (const auto& [entity_id, entity_data] : entity_pack) {
      entities_collection_.Add(entity_id, entity_data);
    }
    // TODO: check if this will be required
//    BringEntitiesToTop();
  });

  message_processor_.OnMessage(client_id_message, [this](const nlohmann::json& message) {
    client_id_ = message;
  });

  {
    nlohmann::json entities_pack;

    nlohmann::json commander;
    commander["name"] = "commander";
    commander["index"] = 5;
    commander["health"] = 50;
    nlohmann::json entity1;
    entity1.push_back(0);
    entity1.push_back(commander);

    nlohmann::json golem;
    golem["name"] = "golem";
    golem["index"] = 24;
    golem["health"] = 50;
    golem["power"] = 25;
    nlohmann::json entity2;
    entity2.push_back(1);
    entity2.push_back(golem);

    entities_pack["entities_pack"].push_back(entity1);
    entities_pack["entities_pack"].push_back(entity2);

    message_processor_.Process(entities_pack);
  }
  {
    // Test client id
    nlohmann::json client_id;
    client_id["client_id"] = 0;
    message_processor_.Process(client_id);
    if (client_id_ != 0) {
      throw std::invalid_argument("Wrong client id");
    }
  }

  panel_->SetCurrentEntity();

  hint_ = MakeHint(window_.GetRenderer(), "This is a hint, which describes how this ability work for this entity");

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

  animation_ = std::make_unique<MoveAnimation>();
  animation_->to_index = 17;
  std::cout << "handle\n";
  animation_->Handle(entities_collection_.Get(0));
  std::cout << "handle2\n";

  scale_animation_ = std::make_unique<ScaleAnimation>();
  scale_animation_->Handle(entities_collection_.Get(1));

  transparency_animation_ = std::make_unique<TransparencyAnimation>();
  transparency_animation_->Handle(entities_collection_.Get(1));
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
  hint_->Draw(window_);
  window_.Display();
}

void Game::Update(std::chrono::milliseconds delta_time) {
  if (animation_ && animation_->Update(delta_time)) {
    animation_ = nullptr;
  }
  if (scale_animation_ && scale_animation_->Update(delta_time)) {
    scale_animation_ = nullptr;
  }
  if (transparency_animation_ && transparency_animation_->Update(delta_time)) {
    transparency_animation_ = nullptr;
  }
}
