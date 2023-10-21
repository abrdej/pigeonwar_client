#pragma once

#include <chrono>
#include <memory>

#include <window.h>
#include <texture_loader.h>
#include <entities_collection.h>
#include <board.h>
#include <panel.h>
#include <animation.h>
#include <text.h>

class Game {
 public:
  Game();
  void Run();
  bool Exit() {
    return !window_.IsOpen();
  }
  void ExecuteLoop();

 private:
  void Render();
  void Update(std::chrono::milliseconds delta_time);

  std::chrono::system_clock::time_point last_update_;
  std::chrono::milliseconds time_since_update_{0};
  std::chrono::milliseconds time_per_frame_{100};

  Window window_;
  TextureLoader texture_loader_{window_};
  EntitiesCollection entities_collection_;
  std::unique_ptr<Board> board_;
  std::unique_ptr<Panel> panel_;
  std::unique_ptr<Text> hint_;

  std::unique_ptr<MoveAnimation> animation_;
  std::unique_ptr<ScaleAnimation> scale_animation_;
  std::unique_ptr<TransparencyAnimation> transparency_animation_;
};
