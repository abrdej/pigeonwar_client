#pragma once

#include <chrono>
#include <functional>
#include <iostream>

class TimerOnUpdate {
 public:
  using CallbackType = std::function<void()>;

  TimerOnUpdate() = default;

  TimerOnUpdate(CallbackType callback, std::chrono::milliseconds timeout)
      : callback_(std::move(callback)), timeout_point_(std::chrono::steady_clock::now() + timeout) {
  }

  ~TimerOnUpdate() {
    suspended_ = false;
  }

  bool Update() {
    if (suspended_) {
      return true;
    }
    if (std::chrono::steady_clock::now() >= timeout_point_) {
      if (callback_) {
        callback_();
      }
      suspended_ = true;
      return true;
    }
    return false;
  }

 private:
  CallbackType callback_;
  std::chrono::steady_clock::time_point timeout_point_;
  bool suspended_{false};
};
