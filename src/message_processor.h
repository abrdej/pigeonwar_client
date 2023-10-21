#pragma once

#include <entity.h>
#include <data_packs.h>

#include <nlohmann/json.hpp>

#include <unordered_map>

static constexpr auto client_id_message = "client_id";
static constexpr auto entities_pack_message = "entities_pack";
static constexpr auto local_state_message = "local_state";
static constexpr auto global_state_message = "game_state";
static constexpr auto animation_message = "animation";
static constexpr auto remove_entity_message = "remove_entity";
static constexpr auto description_message = "description";
static constexpr auto entity_description_message = "entity_description";
static constexpr auto effect_description_message = "effect_description";
static constexpr auto move_entity_message = "move_entity";
static constexpr auto entity_talk_message = "entity_talk";
static constexpr auto defeat_message = "defeat";
static constexpr auto victory_message = "victory";
static constexpr auto game_ready_message = "game_ready";

class Board;
class Panel;
using Entities = std::unordered_map<std::int32_t, std::shared_ptr<Entity>>;

class MessageProcessor {
 public:
  using MessageDataType = nlohmann::json;
  using CallbackType = std::function<void(const MessageDataType& message)>;

  void OnMessage(const std::string& message_type, CallbackType callback);
  void Process(const MessageDataType& message);

 private:
  std::unordered_map<std::string, CallbackType> callbacks_;
};
