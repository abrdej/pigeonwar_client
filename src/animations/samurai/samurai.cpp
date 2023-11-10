#include <animation_plugin_provider_interface.h>

#include <boost/dll/alias.hpp>

#include <animations/animation.h>
#include <animations/animation_helper.h>
#include <entities_collection.h>

struct SwordBlowAnimation : FlushTexture {
  explicit SwordBlowAnimation(Entity& entity)
      : FlushTexture(entity, "samurai_sword_blow", std::chrono::milliseconds(150)) {}

  static constexpr auto name = "sword_blow";
};

struct DodgeAnimation : FlushTexture {
  explicit DodgeAnimation(Entity& entity)
      : FlushTexture(entity, "samurai_dodge", std::chrono::milliseconds(250)) {}

  static constexpr auto name = "dodge";
};

template <>
AnimationInterfacePtr AnimationTranslator<SwordBlowAnimation>(GameHandler& game_handler, const DataType& data) {
  IndexType entity_id = data[1];
  return std::make_unique<SwordBlowAnimation>(game_handler.GetEntitiesCollection().Get(entity_id));
}

template <>
AnimationInterfacePtr AnimationTranslator<DodgeAnimation>(GameHandler& game_handler, const DataType& data) {
  IndexType entity_id = data[1];
  return std::make_unique<DodgeAnimation>(game_handler.GetEntitiesCollection().Get(entity_id));
}

using SamuraiAnimationProvider = AnimationsPluginProvider<SwordBlowAnimation, DodgeAnimation>;

BOOST_DLL_ALIAS(CreateAnimationProvider<SamuraiAnimationProvider>, animation_plugin_provider)
