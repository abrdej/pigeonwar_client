#include <animation_plugin_provider_interface.h>

#include <boost/dll/alias.hpp>

#include <animations/animation.h>
#include <animations/animation_helper.h>
#include <entities_collection.h>

struct MagicBulletAnimation : QueuedFactoryAnimation {
  MagicBulletAnimation(Entity& entity, const TextureLoader& texture_loader, int x, int y) : QueuedFactoryAnimation() {
    ScheduleBack([&entity]() {
      return std::make_unique<FlushTexture>(entity, "monk_use_1", std::chrono::milliseconds(100));
    });
    ScheduleBack([&entity]() {
      return std::make_unique<FlushTexture>(entity, "monk_use_2", std::chrono::milliseconds(100));
    });
    ScheduleBack([&entity]() {
      return std::make_unique<FlushTexture>(entity, "monk_use_3", std::chrono::milliseconds(100));
    });
    ScheduleBack([&texture_loader, x, y]() {
      return std::make_unique<ShowTexture>(texture_loader.GetTexture("magic_splash"),
                                           std::chrono::milliseconds(150),
                                           x, y);
    });
  }

  constexpr static auto name = "magic_bullet";
};

template <>
AnimationInterfacePtr AnimationTranslator<MagicBulletAnimation>(GameHandler& game_handler, const DataType& data) {
  EntityIdType entity_id = data[1];
  IndexType target_index = data[2];
  const auto [x, y] = IndexToPos(target_index);
  return std::make_unique<MagicBulletAnimation>(game_handler.GetEntitiesCollection().Get(entity_id),
                                                game_handler.GetTextureLoader(),
                                                x, y);
}


using MonkAnimationProvider =
    AnimationsPluginProvider<MagicBulletAnimation>;

BOOST_DLL_ALIAS(CreateAnimationProvider<MonkAnimationProvider>, animation_plugin_provider)
