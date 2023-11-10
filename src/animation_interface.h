#pragma once

#include <memory>

#include <drawable.h>
#include <updatable.h>

struct AnimationInterface : UpdatableInterface, DrawableInterface {};

struct DummyDrawAnimation : AnimationInterface {
  void Draw(Window& window) const final {}
};

using AnimationInterfacePtr = std::unique_ptr<AnimationInterface>;
