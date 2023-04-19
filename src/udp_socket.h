#ifndef CHAT_UDP_SOCKET_H
#define CHAT_UDP_SOCKET_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ws2tcpip.h>
#include <winsock2.h>

#include "endpoint.h"

namespace chat {

class UdpSocket {
  SOCKET s;

 public:
  UdpSocket(const int port);
  ~UdpSocket();
  int read(char* buf, int length);
  void write(const Endpoint& endpoint, const char* buf, int length);
};

}  // namespace chat
#endif  // CHAT_UDP_SOCKET_H