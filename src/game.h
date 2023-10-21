#pragma once

#include <chrono>
#include <queue>
#include <memory>

#include <animation.h>
#include <board.h>
#include <entities_collection.h>
#include <game_state.h>
#include <message_processor.h>
#include <panel.h>
#include <text.h>
#include <texture_loader.h>
#include <timer.h>
#include <websocket_client.h>
#include <window.h>

class Game {
  using MessageType = nlohmann::json;

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
  void OnEntityPack(const MessageType& message);
  void OnLocalState(const MessageType& message);
  void OnGlobalState(const MessageType& message);
  void OnAnimation(const MessageType& message);
  void UpdateBoardState();
  void UpdatePanelState();
  void EnqueueTalk(const std::string& text, std::chrono::milliseconds hold);
  void EnqueueCallback(std::function<void()> callback);
  void ProcessCallbacks();
  void ProcessMessage(const std::string& message);
  void SendMessage(const std::string& message);

  std::chrono::system_clock::time_point last_update_;
  std::chrono::milliseconds time_since_update_{0};
  std::chrono::milliseconds time_per_frame_{100};

  ClientIdType client_id_{undefined_client_id};
  bool game_ready_{false};

  Window window_;
  TextureLoader texture_loader_{window_};
  EntitiesCollection entities_collection_;
  LocalGameState local_state_;
  GlobalGameState global_state_;
  std::unique_ptr<Board> board_;
  std::unique_ptr<Panel> panel_;
  std::unique_ptr<Text> hint_;
  std::unique_ptr<Text> talk_;
  MessageProcessor message_processor_;
  std::queue<std::pair<std::string, std::chrono::milliseconds>> text_to_talk_;
  std::optional<std::pair<TimerOnUpdate, int>> hint_timer_;
  std::optional<TimerOnUpdate> talk_timer_;
  std::queue<std::function<void()>> callbacks_;

  std::unique_ptr<MoveAnimation> animation_;
  std::unique_ptr<ScaleAnimation> scale_animation_;
  std::unique_ptr<TransparencyAnimation> transparency_animation_;

  WebsocketClient client_;
};
