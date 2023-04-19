#include <iostream>
#include <regex>
#include <string>

#include "chat_client.h"
#include "chat_view.h"
#include "exception.h"
#include "log.h"
#include "stun_client.h"
#include "udp_socket.h"

chat::Endpoint readPeerPublicEndpointFromInput();

int main(int argc, char argv[]) {
  std::condition_variable finished;
  try {
    chat::UdpSocket socket(7000);
    chat::StunClient stun_client(socket);
    chat::Endpoint stun_server{"64.233.163.127", 19302};  // stun.l.google.com
    auto [public_ip, public_port] = stun_client.getMappedAddress(stun_server);

    util::log("Give your public endpoint to your friend, ");
    util::log("and ask him to send his public enpoint.\n");
    // util::log("Your private endpoint: null:null\n");
    util::log("Your public endpoint: {}:{}\n", public_ip, public_port);
    util::log("Peer public endpoint: ");

    chat::Endpoint peer_address = readPeerPublicEndpointFromInput();

    util::log("Connecting...\n");
    chat::ChatClient chat_client(socket);
    chat_client.connect(peer_address);

    chat::ChatView view;
    view.render();

    // Separate thread to receive messages
    std::thread([&] {
      while (1) {
        auto [recieved, msg] = chat_client.receiveMsg();
        if (recieved) view.addRemoteMessage(msg);
      }
    }).detach();

    // Send messages in current thread
    while (1) {
      std::string input;
      std::cin >> input;
      chat_client.send(input, peer_address);
      view.addLocalMessage(input);
    }

  } catch (const chat::ValidationException& e) {
    util::log("ValidationException: {}\n", e.what());
    finished.notify_one();

  } catch (const chat::StunServerUnavalableException& e) {
    util::log("StunServerUnavalableException: {}\n", e.what());
    finished.notify_one();

  } catch (const chat::ConnectionException& e) {
    util::log("ConnectionException: {}\n", e.what());
    finished.notify_one();

  } catch (const chat::UnableToSendException& e) {
    util::log("UnableToSendException: {}\n", e.what());
    finished.notify_one();

  } catch (const chat::UnableToReceiveException& e) {
    util::log("UnableToReceiveException: {}\n", e.what());
    finished.notify_one();

  } catch (...) {
    util::log("Unknown error\n");
    finished.notify_one();
  }
  return 0;
}

void validateRemoteHostPort(const std::string& input) {
  const std::regex regex(
      "^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9]"
      "[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5]):[0-9]+$");
  if (!std::regex_match(input, regex)) throw chat::ValidationException(input);
}

chat::Endpoint readPeerPublicEndpointFromInput() {
  std::string input;
  std::cin >> input;
  validateRemoteHostPort(input);

  auto colon = input.find(':');
  std::string remote_host = input.substr(0, colon);
  int remote_port = std::stoi(input.substr(colon + 1));
  return {remote_host, remote_port};
}
