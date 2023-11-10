#pragma once

#include <functional>
#include <memory>

#include <boost/dll/alias.hpp>
#include <nlohmann/json.hpp>

#include <animation_interface.h>
#include <game_handler.h>

using DataType = nlohmann::json;

using AnimationCreator = std::function<AnimationInterfacePtr(const DataType&)>;

struct BOOST_SYMBOL_VISIBLE AnimationPluginProviderInterface {
  using NamedAnimationCreator = std::pair<std::string, AnimationCreator>;
  using NamedAnimationCreators = std::vector<NamedAnimationCreator>;
  virtual NamedAnimationCreators GetAnimationsCreators(GameHandler& game_handler) const = 0;
  virtual ~AnimationPluginProviderInterface() = default;
};

using AnimationPluginProviderInterfacePtr = std::shared_ptr<AnimationPluginProviderInterface>;
