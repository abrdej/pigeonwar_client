#pragma once

#include <chrono>
#include <memory>

#include <window.h>
#include <texture_loader.h>
#include <entity.h>

class Game {
 public:
  Game();
  void Run();

 private:
  void Render();
  void Update(std::chrono::milliseconds delta_time);

  Window window_;
  TextureLoader texture_loader_{window_};
  std::shared_ptr<Entity> entity_;
  std::shared_ptr<Entity> entity2_;
};
