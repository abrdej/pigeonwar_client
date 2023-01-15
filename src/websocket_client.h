#pragma once

#include <functional>
#include <memory>
#include <string>

class WebsocketClient {
 public:
  using VoidCallback = std::function<void()>;
  using OnErrorCallback = std::function<void(const std::string&)>;
  using OnMessageCallback = std::function<void(const std::string&)>;

  WebsocketClient();
  ~WebsocketClient();
  void Connect(const std::string& address);
  void Send(const std::string& message);
  void OnOpen(VoidCallback callback);
  void OnMessage(OnMessageCallback callback);
  void OnError(OnErrorCallback callback);
  void OnClose(VoidCallback callback);

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
