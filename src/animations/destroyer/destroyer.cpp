#include <animation_plugin_provider_interface.h>

#include <boost/dll/alias.hpp>

#include <animations/animation.h>
#include <animations/animation_helper.h>
#include <entities_collection.h>

struct BlowTheAxAnimation : MoveAndReturnBaseAnimation {
  BlowTheAxAnimation(Entity& entity,
                     IndexType target_index)
      : MoveAndReturnBaseAnimation(entity, target_index, 0.01f, std::nullopt, "destroyer_attack") {
  }

  constexpr static auto name = "blow_the_ax";
};

template <>
AnimationInterfacePtr AnimationTranslator<BlowTheAxAnimation>(GameHandler& game_handler, const DataType& data) {
  EntityIdType entity_id = data[1];
  IndexType target_index = data[2];
  return std::make_unique<BlowTheAxAnimation>(game_handler.GetEntitiesCollection().Get(entity_id), target_index);
}

using BlowTheAxAnimationProvider = AnimationsPluginProvider<BlowTheAxAnimation>;

BOOST_DLL_ALIAS(CreateAnimationProvider<BlowTheAxAnimationProvider>, animation_plugin_provider)
