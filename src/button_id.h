#pragma once

#include <utils.h>

#include <variant>

struct EntityButtonId {};

struct AbilityButtonId {
  int index{0};
};

struct EffectButtonId {
  int index{0};
};

struct EndTurnButtonId {};

using ButtonId = std::variant<EntityButtonId, AbilityButtonId, EffectButtonId, EndTurnButtonId>;

template <typename T>
inline bool Is(ButtonId& button_id) {
  return std::holds_alternative<T>(button_id);
}

inline bool operator==(const EntityButtonId&, const EntityButtonId&) {
  return true;
}

inline bool operator==(const AbilityButtonId& a, const AbilityButtonId& b) {
  return a.index == b.index;
}

inline bool operator==(const EffectButtonId& a, const EffectButtonId& b) {
  return a.index == b.index;
}

inline bool operator==(const EndTurnButtonId&, const EndTurnButtonId&) {
  return true;
}
