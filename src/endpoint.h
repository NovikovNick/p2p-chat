#ifndef CHAT_ENDPOINT_H
#define CHAT_ENDPOINT_H
#include <string>

namespace chat {

struct Endpoint {
  std::string ip;
  int port;
};

}  // namespace chat
#endif  // CHAT_ENDPOINT_H