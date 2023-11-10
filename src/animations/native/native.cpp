#include <animation_plugin_provider_interface.h>

#include <boost/dll/alias.hpp>

#include <animations/animation.h>
#include <animations/animation_helper.h>
#include <entities_collection.h>

struct DrainAnimation : MoveAndReturnBaseAnimation {
  DrainAnimation(Entity& entity,
                     IndexType target_index)
      : MoveAndReturnBaseAnimation(entity, target_index, 0.01f, std::nullopt, "native_attack") {
  }

  constexpr static auto name = "drain";
};

struct CounterattackAnimation : MoveAndReturnBaseAnimation {
  CounterattackAnimation(Entity& entity,
                 IndexType target_index)
      : MoveAndReturnBaseAnimation(entity, target_index, 0.01f, std::nullopt, "native_attack") {
  }

  constexpr static auto name = "counterattack";
};

template <>
AnimationInterfacePtr AnimationTranslator<DrainAnimation>(GameHandler& game_handler, const DataType& data) {
  EntityIdType entity_id = data[1];
  IndexType target_index = data[2];
  return std::make_unique<DrainAnimation>(game_handler.GetEntitiesCollection().Get(entity_id), target_index);
}

template <>
AnimationInterfacePtr AnimationTranslator<CounterattackAnimation>(GameHandler& game_handler, const DataType& data) {
  EntityIdType entity_id = data[1];
  IndexType target_index = data[2];
  return std::make_unique<CounterattackAnimation>(game_handler.GetEntitiesCollection().Get(entity_id), target_index);
}

using NativeAnimationProvider = AnimationsPluginProvider<DrainAnimation, CounterattackAnimation>;

BOOST_DLL_ALIAS(CreateAnimationProvider<NativeAnimationProvider>, animation_plugin_provider)
