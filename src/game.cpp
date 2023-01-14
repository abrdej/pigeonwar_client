#include <game.h>

#include <chrono>
#include <iostream>

Game::Game() {
  texture_loader_.LoadTexture("commander", "/home/abrdej/Projects/Pigeonwar/pigeon-war-client/res/commander.png");
  texture_loader_.LoadTexture("golem", "/home/abrdej/Projects/Pigeonwar/pigeon-war-client/res/golem.png");

  EntityProperties entity_properties;
  entity_properties.name = "commander";
  entity_properties.index = 5;
  entity_ = std::make_shared<Entity>(texture_loader_, entity_properties);

  entity_properties.name = "golem";
  entity_properties.index = 24;
  entity2_ = std::make_shared<Entity>(texture_loader_, entity_properties);
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
  entity_->Draw(window_);
  entity2_->Draw(window_);
  window_.Display();
}

void Game::Update(std::chrono::milliseconds delta_time) {

}
