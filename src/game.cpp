#include <game.h>

#include <chrono>
#include <iostream>

#include <index_pos_conversion.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

Game::Game() {
  texture_loader_.LoadTexture("commander");
  texture_loader_.LoadTexture("golem");
  texture_loader_.LoadTexture("grass");
  texture_loader_.LoadTexture("border");

  board_ = std::make_unique<Board>(texture_loader_, 15, 10);
  panel_ = std::make_unique<Panel>(texture_loader_, 150, 10 * 60 + 10, 10);
  EntityProperties entity_properties;
  entity_properties.name = "commander";
  entity_properties.index = 5;
  entity_ = std::make_shared<Entity>(texture_loader_, entity_properties);

  text_ = std::make_unique<Text>(window_.GetRenderer());
  text_->SetPos(50, 400);
  text_->SetText("Hello text");

  entity_properties.name = "golem";
  entity_properties.index = 24;
  entity2_ = std::make_shared<Entity>(texture_loader_, entity_properties);

  window_.OnMousePressed([this](int x, int y) {
    if (board_->Clicked(x, y)) {
      std::int32_t index;
      PosToIndex(x, y, index);
      std::cout << "Clicked board on index: " << index << "\n";

      // TODO: send message to server

    } else if (panel_->Clicked(x, y)) {
      std::cout << "Panel clicked\n";
    }

    panel_->InteractPress(x, y);
  });

  window_.OnMouseMove([this](int x, int y) {
    panel_->InteractMove(x, y);
  });

  animation_ = std::make_unique<MoveAnimation>();
  animation_->to_index = 17;
  std::cout << "handle\n";
  animation_->Handle(*entity_);
  std::cout << "handle2\n";
}

void LoopHandler(void* game) {
  Game* this_game = reinterpret_cast<Game*>(game);
  this_game->ExecuteLoop();
}

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
  entity_->Draw(window_);
  entity2_->Draw(window_);
  text_->Draw(window_);
  window_.Display();
}

void Game::Update(std::chrono::milliseconds delta_time) {
  if (animation_ && animation_->Update(delta_time)) {
    animation_ = nullptr;
  }
}
