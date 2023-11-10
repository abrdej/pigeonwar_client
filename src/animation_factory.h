#pragma once

#include <nlohmann/json.hpp>

#include <animation_interface.h>
#include <game_handler.h>

class AnimationFactory {
 public:
  using DataType = nlohmann::json;

  AnimationFactory();
  ~AnimationFactory();
  AnimationInterfacePtr Create(const DataType& data);
  void LoadAnimations(GameHandler& game_handler);

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
