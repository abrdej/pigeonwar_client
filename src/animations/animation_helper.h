#pragma once

#include <nlohmann/json.hpp>

#include <animation_interface.h>
#include <animation_plugin_provider_interface.h>
#include <game_handler.h>

using DataType = nlohmann::json;

template <typename Animation>
AnimationInterfacePtr AnimationTranslator(GameHandler& game_handler, const DataType& data);

using NamedAnimationCreator = AnimationPluginProviderInterface::NamedAnimationCreator;

template <typename T>
NamedAnimationCreator MakeNamedCreator(GameHandler& game_handler) {
  return {T::name, [&game_handler](const DataType& data) {
    return AnimationTranslator<T>(game_handler, data);
  }};
}

template <typename... Animations>
struct AnimationsPluginProvider : AnimationPluginProviderInterface {
  NamedAnimationCreators GetAnimationsCreators(GameHandler& game_handler) const override {
    return {
        MakeNamedCreator<Animations>(game_handler)...
    };
  }
};

template <typename Provider>
std::shared_ptr<Provider> CreateAnimationProvider() {
  return std::make_shared<Provider>();
}
