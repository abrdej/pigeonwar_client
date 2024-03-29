#pragma once

#include <chrono>
#include <queue>
#include <memory>

#include <animations/animation.h>
#include <animation_factory.h>
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
  void OnGetHint(ButtonId button_id);
  void ClearSelectedIndex();

  std::chrono::steady_clock::time_point last_update_;
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
  std::optional<std::pair<TimerOnUpdate, ButtonId>> hint_timer_;
  bool hint_requested_{false};
  std::optional<TimerOnUpdate> talk_timer_;
  std::queue<std::function<void()>> callbacks_;

  AnimationFactory animation_factory_;
  std::vector<std::unique_ptr<AnimationInterface>> animations_;

  std::unique_ptr<ScaleAnimation> scale_animation_;
  std::unique_ptr<TransparencyAnimation> transparency_animation_;

  WebsocketClient client_;

  struct GameHandlerImpl : GameHandler {
    explicit GameHandlerImpl(Game& game) : game_(game) {}

    TextureLoader& GetTextureLoader() override {
      return game_.texture_loader_;
    }

    EntitiesCollection& GetEntitiesCollection() override {
      return game_.entities_collection_;
    }

    void ClearSelectedIndex() override {
      game_.ClearSelectedIndex();
    }

    Renderer GetRenderer() override {
      return game_.window_.GetRenderer();
    }

    Game& game_;
  };

  std::unique_ptr<GameHandlerImpl> game_handler_;
};
