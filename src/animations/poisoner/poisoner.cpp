#include <animation_plugin_provider_interface.h>

#include <boost/dll/alias.hpp>

#include <animations/animation.h>
#include <animations/animation_helper.h>

struct PoisonedMissileAnimation : ShotBaseAnimation {
  PoisonedMissileAnimation(TextureLoader& texture_loader,
                           IndexType source_index,
                           IndexType target_index)
      : ShotBaseAnimation(texture_loader, source_index, target_index, 0.0125,
                          "poisoned_missile", "poisoned_missile_explosion",
                          std::chrono::milliseconds(100)) {}

  static constexpr auto name = "poisoned_missile";
};

template <>
AnimationInterfacePtr AnimationTranslator<PoisonedMissileAnimation>(GameHandler& game_handler, const DataType& data) {
  IndexType source_index = data[1];
  IndexType target_index = data[2];
  return std::make_unique<PoisonedMissileAnimation>(game_handler.GetTextureLoader(), source_index, target_index);
}

using ShooterAnimationProvider = AnimationsPluginProvider<PoisonedMissileAnimation>;

BOOST_DLL_ALIAS(CreateAnimationProvider<ShooterAnimationProvider>, animation_plugin_provider)
