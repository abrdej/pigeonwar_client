#include <animation_plugin_provider_interface.h>

#include <boost/dll/alias.hpp>

#include <animations/animation.h>
#include <animations/animation_helper.h>

struct ShotAnimation : ShotBaseAnimation {
  ShotAnimation(TextureLoader& texture_loader,
                IndexType source_index,
                IndexType target_index)
      : ShotBaseAnimation(texture_loader, source_index, target_index, 0.0175, "bullet", "bum",
                          std::chrono::milliseconds(150)) {}

  // TODO: fix the name
  static constexpr auto name = "shoot";
};

template <>
AnimationInterfacePtr AnimationTranslator<ShotAnimation>(GameHandler& game_handler, const DataType& data) {
  IndexType source_index = data[1];
  IndexType target_index = data[2];
  return std::make_unique<ShotAnimation>(game_handler.GetTextureLoader(), source_index, target_index);
}

using ShooterAnimationProvider = AnimationsPluginProvider<ShotAnimation>;

BOOST_DLL_ALIAS(CreateAnimationProvider<ShooterAnimationProvider>, animation_plugin_provider)
