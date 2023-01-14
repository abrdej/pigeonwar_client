#include <game.h>

#include <chrono>
#include <iostream>

#include <index_pos_conversion.h>

Game::Game() {
  texture_loader_.LoadTexture("commander", "/home/abrdej/Projects/Pigeonwar/pigeon-war-client/res/commander.png");
  texture_loader_.LoadTexture("golem", "/home/abrdej/Projects/Pigeonwar/pigeon-war-client/res/golem.png");
  texture_loader_.LoadTexture("grass", "/home/abrdej/Projects/Pigeonwar/pigeon-war-client/res/grass.png");

  board_ = std::make_unique<Board>(texture_loader_, 15, 10);

  EntityProperties entity_properties;
  entity_properties.name = "commander";
  entity_properties.index = 5;
  entity_ = std::make_shared<Entity>(texture_loader_, entity_properties);

  entity_properties.name = "golem";
  entity_properties.index = 24;
  entity2_ = std::make_shared<Entity>(texture_loader_, entity_properties);

  window_.OnMousePressed([this](int x, int y) {
    if (board_->Clicked(x, y)) {
      std::int32_t index;
      PosToIndex(x, y, index);
      std::cout << "Clicked board on index: " << index << "\n";
    }
  });

  animation_ = std::make_unique<MoveAnimation>();
  animation_->to_index = 17;
  std::cout << "handle\n";
  animation_->Handle(*entity_);
  std::cout << "handle2\n";
}

void Game::Run() {
  auto last_time = std::chrono::system_clock::now();
  std::chrono::milliseconds time_since_update(0);
  std::chrono::milliseconds time_per_frame(100);

  while (window_.IsOpen()) {
    window_.ProcessEvents();

    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_time);
    time_since_update += duration;
    while (time_since_update > time_per_frame) {
      last_time = now;
      time_since_update -= time_per_frame;
      window_.ProcessEvents();
      Update(time_per_frame);
    }
    Render();
  }
}

void Game::Render() {
  window_.Clear();
  board_->Draw(window_);
  entity_->Draw(window_);
  entity2_->Draw(window_);
  window_.Display();
}

void Game::Update(std::chrono::milliseconds delta_time) {
  std::cout << "Before update1\n";
  if (animation_ && animation_->Update(delta_time)) {
    animation_ = nullptr;
  }
  std::cout << "Before update2\n";
}
