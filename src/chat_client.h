#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H
#define BUFLEN 1024
#include <string>
#include <utility>

#include "endpoint.h"
#include "udp_socket.h"

namespace chat {

class ChatClient {
  chat::UdpSocket &socket;
  char buf[BUFLEN];

 public:
  ChatClient(UdpSocket &socket);
  void connect(const Endpoint &peer_endpoint);
  void send(std::string msg, const Endpoint &peer_endpoint);
  std::pair<bool, std::string> receiveMsg();
};
}  // namespace chat
#endif  // CHAT_CLIENT_H
