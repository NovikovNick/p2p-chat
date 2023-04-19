#ifndef STUN_CLIENT_H
#define STUN_CLIENT_H
#define BUFLEN 1024
#include "endpoint.h"
#include "udp_socket.h"

namespace chat {

class StunClient {
  chat::UdpSocket &socket;
  char buf[BUFLEN];

 public:
  StunClient(UdpSocket &socket);
  Endpoint getMappedAddress(const Endpoint &stun_server_endpoint);
};
}  // namespace chat
#endif  // STUN_CLIENT_H
