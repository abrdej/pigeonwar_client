#include <websocket_client.h>

#include <stdexcept>

#ifdef EMSCRIPTEN
#include <emscripten/websocket.h>
#endif

struct Callbacks {
  using VoidCallback = std::function<void()>;
  using OnErrorCallback = std::function<void(const std::string&)>;
  using OnMessageCallback = std::function<void(const std::string&)>;

  VoidCallback on_open_callback;
  OnMessageCallback on_message_callback;
  OnErrorCallback on_error_callback;
  VoidCallback on_close_callback;
};

EM_BOOL OnOpen(int event_type, const EmscriptenWebSocketOpenEvent *websocket_event, void *user_data) {
  auto callbacks = reinterpret_cast<Callbacks*>(user_data);
  if (callbacks->on_open_callback) {
    callbacks->on_open_callback();
  }
  return EM_TRUE;
}

EM_BOOL OnMessage(int event_type, const EmscriptenWebSocketMessageEvent *websocket_event, void *user_data) {
  if (websocket_event->isText) {
    auto callbacks = reinterpret_cast<Callbacks*>(user_data);
    if (callbacks->on_message_callback) {
      callbacks->on_message_callback(std::string(websocket_event->data));
    }
  }
  return EM_TRUE;
}

EM_BOOL OnError(int event_type, const EmscriptenWebSocketErrorEvent *websocket_event, void *user_data) {
  auto callbacks = reinterpret_cast<Callbacks*>(user_data);
  if (callbacks->on_error_callback) {
    // TODO: how to get error message
    callbacks->on_error_callback(std::string());
  }
  return EM_TRUE;
}

EM_BOOL OnClose(int event_type, const EmscriptenWebSocketCloseEvent *websocket_event, void *user_data) {
  auto callbacks = reinterpret_cast<Callbacks*>(user_data);
  if (callbacks->on_close_callback) {
    // TODO: you can add more info here
    callbacks->on_close_callback();
  }
  return EM_TRUE;
}

struct WebsocketClient::Impl {
  Impl();
  void Connect(const std::string& address);
  void Send(const std::string& message);
  void OnOpen(VoidCallback callback);
  void OnMessage(OnMessageCallback callback);
  void OnError(OnErrorCallback callback);
  void OnClose(VoidCallback callback);

  EMSCRIPTEN_WEBSOCKET_T ws_;
  std::string address_;
  Callbacks callbacks_;
};

WebsocketClient::Impl::Impl() {
  if (!emscripten_websocket_is_supported()) {
    throw std::runtime_error("Web sockets are not supported");
  }
}

void WebsocketClient::Impl::Connect(const std::string& address) {
  address_ = address;
  EmscriptenWebSocketCreateAttributes ws_attrs = {
      address,
      NULL,
      EM_TRUE
  };
  ws_ = emscripten_websocket_new(&ws_attrs);
  if (ws_ < 0) {
    throw std::runtime_error("Failed to connect to server: " + address);
  }
  emscripten_websocket_set_onopen_callback(ws_, &callbacks_, OnOpen);
  emscripten_websocket_set_onerror_callback(ws_, &callbacks_, OnError);
  emscripten_websocket_set_onmessage_callback(ws_, &callbacks_, OnMessage);
  emscripten_websocket_set_onclose_callback(ws_, &callbacks_, OnClose);
}

void WebsocketClient::Impl::Send(const std::string& message) {
  EMSCRIPTEN_RESULT result = emscripten_websocket_send_utf8_text(ws_, message);
  if (result) {
    throw std::runtime_error("Failed to send message to server: " + address_);
  }
}

void WebsocketClient::Impl::OnOpen(VoidCallback callback) {
  callbacks_.on_open_callback = std::move(callback);
}

void WebsocketClient::Impl::OnMessage(OnMessageCallback callback) {
  callbacks_.on_message_callback = std::move(callback);
}

void WebsocketClient::Impl::OnError(OnErrorCallback callback) {
  callbacks_.on_error_callback = std::move(callback);
}

void WebsocketClient::Impl::OnClose(VoidCallback callback) {
  callbacks_.on_close_callback = std::move(callback);
}

WebsocketClient::WebsocketClient() : impl_(std::make_unique<Impl>()) {}

WebsocketClient::~WebsocketClient() = default;

void WebsocketClient::Connect(const std::string& address) {
  impl_->Connect(address);
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
