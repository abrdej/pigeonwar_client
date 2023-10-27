#pragma once

#include <functional>
#include <optional>

template <typename T, typename F>
auto AndThen(const std::optional<T>& value, F f) -> std::optional<std::invoke_result_t<F, T&>> {
  if (value) {
    return std::invoke(std::forward<F>(f), *value);
  } else {
    return std::nullopt;
  }
}
