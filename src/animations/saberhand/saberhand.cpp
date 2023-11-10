#include <animation_plugin_provider_interface.h>

#include <boost/dll/alias.hpp>

#include <animations/animation.h>
#include <animations/animation_helper.h>
#include <entities_collection.h>

struct SabersAnimation : MoveAndReturnBaseAnimation {
  SabersAnimation(Entity& entity,
                  IndexType target_index)
      : MoveAndReturnBaseAnimation(entity, target_index, 0.01f, std::nullopt, "sabers_attack") {
  }

  constexpr static auto name = "sabers";
};

struct SetInvisibilityAnimation : FunctionWrappedAnimation {
  explicit SetInvisibilityAnimation(Entity& entity)
      : FunctionWrappedAnimation([&entity]() {
    entity.ChangeTexture("saberhand_transparency");
    return true;
  }) {}

  static constexpr auto name = "set_invisibility";
};

struct RemoveInvisibilityAnimation : FunctionWrappedAnimation {
  explicit RemoveInvisibilityAnimation(Entity& entity)
      : FunctionWrappedAnimation([&entity]() {
    entity.RevertTexture();
    return true;
  }) {}

  static constexpr auto name = "remove_invisibility";
};

template <>
AnimationInterfacePtr AnimationTranslator<SabersAnimation>(GameHandler& game_handler, const DataType& data) {
  EntityIdType entity_id = data[1];
  IndexType target_index = data[2];
  return std::make_unique<SabersAnimation>(game_handler.GetEntitiesCollection().Get(entity_id), target_index);
}

template <>
AnimationInterfacePtr AnimationTranslator<SetInvisibilityAnimation>(GameHandler& game_handler, const DataType& data) {
  IndexType entity_id = data[1];
  return std::make_unique<SetInvisibilityAnimation>(game_handler.GetEntitiesCollection().Get(entity_id));
}

template <>
AnimationInterfacePtr AnimationTranslator<RemoveInvisibilityAnimation>(GameHandler& game_handler,
                                                                       const DataType& data) {
  IndexType entity_id = data[1];
  return std::make_unique<RemoveInvisibilityAnimation>(game_handler.GetEntitiesCollection().Get(entity_id));
}

using SaberhandAnimationProvider =
    AnimationsPluginProvider<SabersAnimation, SetInvisibilityAnimation, RemoveInvisibilityAnimation>;

BOOST_DLL_ALIAS(CreateAnimationProvider<SaberhandAnimationProvider>, animation_plugin_provider)
