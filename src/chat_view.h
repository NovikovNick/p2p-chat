#ifndef CHAT_H
#define CHAT_H
#include <format>
#include <string>
#include <vector>

#include "log.h"

namespace chat {

class ChatView {
  std::mutex m;
  std::vector<std::string> chat_messages;

 public:
  void addLocalMessage(const std::string& msg) {
    std::scoped_lock lock(m);
    chat_messages.push_back(std::format("You : {}", msg));
    render();
  }

  void addRemoteMessage(const std::string& msg) {
    std::scoped_lock lock(m);
    chat_messages.push_back(std::format("Peer: {}", msg));
    render();
  }

  void render() {
    clearConsole();
    util::log("Peer connected!\n");
    for (const auto& msg : chat_messages) util::log(msg + "\n");
    util::log("Type message to peer: ");
  }

 private:
  void clearConsole() { system("cls"); }
};

}  // namespace chat
#endif  // CHAT_H
