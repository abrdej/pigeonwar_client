#include <animation_plugin_provider_interface.h>

#include <boost/dll/alias.hpp>

#include <animations/animation.h>
#include <animations/animation_helper.h>
#include <entities_collection.h>

struct SniperShotAnimation : ShotBaseAnimation {
  SniperShotAnimation(TextureLoader& texture_loader,
                      IndexType source_index,
                      IndexType target_index)
      : ShotBaseAnimation(texture_loader, source_index, target_index, 0.02, "sniper_shot", "sniper_bullet_explosion",
                          std::chrono::milliseconds(150)) {}

  static constexpr auto name = "sniper_shot";
};

struct HypnosisAnimation : ShowTexture {
  HypnosisAnimation(const Texture& texture,
                    int x, int y)
      : ShowTexture(texture, std::chrono::milliseconds(150), x, y) {}

  constexpr static auto name = "hypnosis";
};

template <>
AnimationInterfacePtr AnimationTranslator<SniperShotAnimation>(GameHandler& game_handler, const DataType& data) {
  IndexType source_index = data[1];
  IndexType target_index = data[2];
  return std::make_unique<SniperShotAnimation>(game_handler.GetTextureLoader(), source_index, target_index);
}

template <>
AnimationInterfacePtr AnimationTranslator<HypnosisAnimation>(GameHandler& game_handler, const DataType& data) {
  const IndexType index = data[1];
  const auto [x, y] = IndexToPos(index);
  return std::make_unique<HypnosisAnimation>(game_handler.GetTextureLoader().GetTexture("hypnosis"), x, y);
}

using SniperAnimationProvider = AnimationsPluginProvider<SniperShotAnimation, HypnosisAnimation>;

BOOST_DLL_ALIAS(CreateAnimationProvider<SniperAnimationProvider>, animation_plugin_provider)
