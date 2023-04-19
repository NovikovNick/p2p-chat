#include "stun_client.h"

#include <stdio.h>
#include <stdlib.h>

#include "exception.h"
#include "log.h"
#include "stun_protocol.h"

namespace chat {

StunClient::StunClient(UdpSocket &socket) : socket(socket) {}

Endpoint StunClient::getMappedAddress(const Endpoint &stun_server_endpoint) {
  
  memset(buf, '\0', BUFLEN);

  stun::PACKET stun_request{};
  stun::PACKET *request_ptr = &stun_request;
  stun::packet_init(request_ptr, buf, BUFLEN);
  stun::stun_write_header(request_ptr, stun::STUN_BINDING_METHOD);
  stun::stun_write_footer(request_ptr);

  socket.write(stun_server_endpoint, request_ptr->buf, request_ptr->len);

  memset(buf, '\0', BUFLEN);

  stun::PACKET stun_response;
  stun::PACKET *response_ptr = &stun_response;
  stun::packet_init(response_ptr, buf, BUFLEN);
  for (int attempt = 0; attempt < 10; ++attempt) {
    if (socket.read(response_ptr->buf, BUFLEN) >= 0)
      return stun::stun_parse(response_ptr);
  }
  throw chat::StunServerUnavalableException();
}
}  // namespace chat
