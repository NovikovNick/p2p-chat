#include "chat_client.h"

#include <stdio.h>
#include <stdlib.h>

#include "exception.h"
#include "log.h"
#include <ctime>


namespace chat {

ChatClient::ChatClient(UdpSocket &socket) : socket(socket) {}

void ChatClient::connect(const Endpoint &peer_endpoint) {
  srand((unsigned)time(NULL) * getpid());  
  std::string connection_id("1234567890");
  for (int i = 0; i < connection_id.size(); i++)
    connection_id[i] = 'a' + rand() % ('z' - 'a');

  for (int attempt = 0; attempt < 120; ++attempt) {
   
    send(connection_id, peer_endpoint);
    auto [received, msg] = receiveMsg();
    if (received) {
      send(msg, peer_endpoint);
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(1s);
      while(receiveMsg().first);
      return;
    };
  }
  throw ConnectionException(666);
}

void ChatClient::send(std::string msg, const Endpoint &peer_endpoint) {
  memset(buf, '\0', BUFLEN);
  strcpy(buf, msg.c_str());
  socket.write(peer_endpoint, buf, msg.size());
};

std::pair<bool, std::string> ChatClient::receiveMsg() {
  memset(buf, '\0', BUFLEN);
  auto recv_length = socket.read(buf, BUFLEN);
  std::string res(buf);
  return {recv_length > 0, res};
};

}  // namespace chat