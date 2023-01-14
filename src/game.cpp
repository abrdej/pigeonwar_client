#include <game.h>

#include <chrono>
#include <iostream>

Game::Game() {
  texture_loader_.LoadTexture("commander", "/home/abrdej/Projects/Pigeonwar/pigeon-war-client/res/commander.png");
  texture_ = std::make_shared<Texture>(texture_loader_.GetTexture("commander"));
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
  texture_->Draw(window_);
  window_.Display();
}

void Game::Update(std::chrono::milliseconds delta_time) {

}
