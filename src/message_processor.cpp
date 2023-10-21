#include <message_processor.h>

#include <board.h>
#include <panel.h>

void MessageProcessor::OnMessage(const std::string& message_type, CallbackType callback) {
  callbacks_.emplace(message_type, callback);
}

void MessageProcessor::Process(const MessageDataType& message) {
  try {
    for (const auto& [message_type, callback] : callbacks_) {
      if (message.contains(message_type)) {
        callbacks_.at(message_type)(message[message_type]);
        break;
      }
    }
  } catch (std::out_of_range& e) {
    throw std::out_of_range(
        "MessageProcessor::Process: no callback registered for this message type: " + std::string(e.what()));
  }
}
