#include "stun_client.h"

#include <stdio.h>
#include <stdlib.h>

#include "exception.h"
#include "log.h"
#include "packet.h"

namespace stun {

StunClient::StunClient(const uint16_t local_port, const char *stun_host,
                       const uint16_t stun_port)
    : local_port_(local_port) {
  WSADATA wsa;
  int res, recv_len, slen = sizeof(stun_addr);

  if ((res = WSAStartup(MAKEWORD(2, 2), &wsa)) != 0)
    throw ConnectionException(WSAGetLastError());

  if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
    throw ConnectionException(WSAGetLastError());

  memset((char *)&stun_addr, 0, sizeof(stun_addr));
  stun_addr.sin_family = AF_INET;
  stun_addr.sin_addr.S_un.S_addr = inet_addr(stun_host);
  stun_addr.sin_port = htons(stun_port);

  util::log("STUN-client from localhost:{} to {}:{}\n", local_port, stun_host,
            stun_port);
}

StunClient::~StunClient() {
  if (s != INVALID_SOCKET) {
    closesocket(s);
    WSACleanup();
  }
}

void StunClient::sendRequest() {
  PACKET mp;
  PACKET *m = &mp;
  packet_init(m, buf, BUFLEN);
  stun_write_header(m, STUN_BINDING_METHOD);
  stun_write_footer(m);

  int recv_len, slen = sizeof(stun_addr);
  if (sendto(s, m->buf, m->len, 0, (sockaddr *)&stun_addr, slen) ==
      SOCKET_ERROR) {
    throw UnableToSendException(WSAGetLastError());
  }

  memset(buf, '\0', BUFLEN);
}

void StunClient::waitResponse() {
  int recv_len, slen = sizeof(stun_addr);
  if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (sockaddr *)&stun_addr, &slen)) ==
      SOCKET_ERROR) {
    throw UnableToReceiveException(WSAGetLastError());
  }

  PACKET mp;
  PACKET *m = &mp;
  packet_init(m, buf, recv_len);
  stun_parse(m);

  memset(buf, '\0', BUFLEN);
}

void StunClient::connect(const char *remote_host, const uint16_t remote_port) {
  closesocket(s);
  s = socket(AF_INET, SOCK_DGRAM, 0);
  int optval = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof optval);
  setsockopt(s, SOL_SOCKET, SO_DONTLINGER, (const char *)&optval,
             sizeof optval);

  // non-blocking...
  u_long iMode = 1;
  ioctlsocket(s, FIONBIO, &iMode);

  memset((char *)&local_addr, 0, sizeof(local_addr));
  local_addr.sin_family = AF_INET;
  local_addr.sin_addr.s_addr = INADDR_ANY;
  local_addr.sin_port = htons(local_port_);

  if (bind(s, (sockaddr *)&local_addr, sizeof(local_addr)) == SOCKET_ERROR)
    throw ConnectionException(WSAGetLastError());

  sockaddr_in remote_addr;
  memset((char *)&remote_addr, 0, sizeof(remote_addr));
  remote_addr.sin_family = AF_INET;
  remote_addr.sin_addr.S_un.S_addr = inet_addr(remote_host);
  remote_addr.sin_port = htons(remote_port);

  int recv_len, slen = sizeof(remote_addr);

  using namespace std::chrono_literals;
  int count = 0;
  while (1) {
    memset(buf, '\0', BUFLEN);
    std::string str(std::format("Packet {}", ++count));
    strcpy(buf, str.c_str());

    if (sendto(s, buf, str.size(), 0, (sockaddr *)&remote_addr, slen) ==
        SOCKET_ERROR) {
      throw UnableToSendException(WSAGetLastError());
    }

    memset(buf, '\0', BUFLEN);
    recv_len = recvfrom(s, buf, BUFLEN, 0, (sockaddr *)&remote_addr, &slen);
    util::log("received {} bytes. {}\n", recv_len, buf);
    std::this_thread::sleep_for(1s);
  }
}
}  // namespace stun
