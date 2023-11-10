#include <animation_factory.h>

#include <fstream>
#include <iostream>

#include <boost/dll/shared_library.hpp>
#include <boost/function.hpp>

#include <animation_plugin_provider_interface.h>
#include <config.h>

std::vector<std::string> GetAnimationPlugins() {
  std::ifstream ifs(config_directory + std::string("animation_plugins.json"));
  try {
    nlohmann::json parsed = nlohmann::json::parse(ifs);
    return parsed;
  } catch (std::exception& e) {
    std::cerr << "failed to parse animation plugins, reason: " << e.what() << "\n";
  }
  return {};
}

struct AnimationFactory::Impl {
  AnimationInterfacePtr Create(const DataType& data) {
    const std::string animation_name = data[0];
    return animation_creators_.at(animation_name)(data);
  }

  void LoadAnimations(GameHandler& game_handler) {
    const auto animation_plugins = GetAnimationPlugins();
    for (const auto& animation_plugin : animation_plugins) {
      boost::dll::fs::path plugin_path(animation_plugin);
      boost::dll::shared_library plugin(plugin_path);
      auto plugin_provider = plugin.get_alias<AnimationPluginProviderInterfacePtr()>("animation_plugin_provider");
      auto creators = plugin_provider()->GetAnimationsCreators(game_handler);
      for (const auto& creator : creators) {
        animation_creators_.emplace(creator);
      }
    }
  }

  std::unordered_map<std::string, AnimationCreator> animation_creators_;
};

AnimationFactory::AnimationFactory() : impl_(std::make_unique<Impl>()) {}

AnimationInterfacePtr AnimationFactory::Create(const DataType& data) {
  return impl_->Create(data);
}

void AnimationFactory::LoadAnimations(GameHandler& game_handler) {
  impl_->LoadAnimations(game_handler);
}

AnimationFactory::~AnimationFactory() = default;
