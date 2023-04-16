#include <iostream>
#include <regex>
#include <string>

#include "exception.h"
#include "stun_client.h"
#include "log.h"

void validateRemoteHostPort(const std::string& input);

int main(int argc, char argv[]) {
  std::string stun_host_str("64.233.163.127");  // stun.l.google.com
  uint16_t local_port = 7000, stun_port = 19302;

  /*
  log("Local port: ");
  std::cin >> local_port;

  log("Stun server host: ");
  std::cin >> stun_host_str;

  log("Stun server port: ");
  std::cin >> remote_port;
  */
  std::condition_variable finished;
  std::thread([&] {
    try {
      stun::StunClient client(local_port, stun_host_str.c_str(), stun_port);
      client.sendRequest();
      client.waitResponse();

      util::log("Remote host:port:");
      std::string input("127.0.0.1:7000");
      std::cin >> input;
      validateRemoteHostPort(input);

      auto colon = input.find(':');
      std::string remote_host = input.substr(0, colon);
      uint16_t remote_port = std::stoi(input.substr(colon + 1));

      client.connect(remote_host.c_str(), remote_port);

    } catch (const stun::ValidationException& e) {
      util::log("ValidationException: {}\n", e.what());
      finished.notify_one();

    } catch (const stun::ConnectionException& e) {
      util::log("ConnectionException: {}\n", e.what());
      finished.notify_one();

    } catch (const stun::UnableToSendException& e) {
      util::log("UnableToSendException: {}\n", e.what());
      finished.notify_one();

    } catch (const stun::UnableToReceiveException& e) {
      util::log("UnableToReceiveException: {}\n", e.what());
      finished.notify_one();

    } catch (...) {
      util::log("Unknown error\n");
      finished.notify_one();
    }
  }).detach();

  using namespace std::chrono_literals;
  std::mutex m;
  std::unique_lock lock(m);
  finished.wait_for(lock, 10min);

  return 0;
}

void validateRemoteHostPort(const std::string& input) {
  const std::regex regex(
      "^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9]"
      "[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5]):[0-9]+$");
  if (!std::regex_match(input, regex)) throw stun::ValidationException(input);
}