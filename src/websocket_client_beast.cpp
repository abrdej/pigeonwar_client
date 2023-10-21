#include <websocket_client.h>

#include <lynx/net/client.h>

struct WebsocketClient::Impl {
  Impl() {

  }

  bool Connect(const std::string& address, int port) {
    return client_.connect(address, port);
  }

  void Send(const std::string& message) {
    client_.send(message);
  }

  void OnOpen(VoidCallback callback) {

  }

  void OnMessage(OnMessageCallback callback) {
    client_.on_message(std::move(callback));
  }

  void OnError(OnErrorCallback callback) {

  }

  void OnClose(VoidCallback callback) {

  }

  void Update(std::size_t max_messages)  {
    client_.update(max_messages);
  }

  lynx::net::client client_;
};

WebsocketClient::WebsocketClient() : impl_(std::make_unique<Impl>()) {}

WebsocketClient::~WebsocketClient() = default;

bool WebsocketClient::Connect(const std::string& address, int port) {
  return impl_->Connect(address, port);
}

void WebsocketClient::Send(const std::string& message) {
  impl_->Send(message);
}

void WebsocketClient::OnOpen(VoidCallback callback) {
  impl_->OnOpen(std::move(callback));
}

void WebsocketClient::OnMessage(OnMessageCallback callback) {
  impl_->OnMessage(std::move(callback));
}

void WebsocketClient::OnError(OnErrorCallback callback) {
  impl_->OnError(std::move(callback));
}

void WebsocketClient::OnClose(VoidCallback callback) {
  impl_->OnClose(std::move(callback));
}

void WebsocketClient::Update(std::size_t max_messages) {
  impl_->Update(max_messages);
}
