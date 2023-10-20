#pragma once

#include <entity.h>

static constexpr auto client_id_message = "client_id";
static constexpr auto entities_pack_message = "entities_pack";
static constexpr auto local_state_message = "local_state";
static constexpr auto game_state_message = "game_state"; // probably global_game_state
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

static constexpr auto no_selected_index = std::numeric_limits<std::int32_t>::max();

struct EntityCollection {
  virtual void Add(std::uint32_t entity_id, const EntityProperties& entity_properties) = 0;
};

struct LocalState {
  std::vector<std::int32_t> possible_movements;
  std::vector<std::int32_t> valid_movements;
  std::int32_t selected_index;
  std::string actual_target_type;
  std::vector<std::string> button_bitmaps;
  bool usable{false};
  std::string selected_entity_name;
};

void OnClientId();

void OnEntitiesPack(EntityCollection& entity_collection);

void OnLocalState(const LocalState& local_state, Board& board, Panel& panel, Entities& entities);

class MessageProcessor {

};
