#ifndef STUN_EXCEPTION_H
#define STUN_EXCEPTION_H
#include <format>
#include <stdexcept>

namespace stun {

// CONNECTION EXCEPTION

class ConnectionException : public std::runtime_error {
 public:
  ConnectionException(const int error_code)
      : std::runtime_error(std::format("Error code {}", error_code)) {}
};

class UnableToReceiveException : public ConnectionException {
 public:
  UnableToReceiveException(const int error_code)
      : ConnectionException(error_code) {}
};

class UnableToSendException : public ConnectionException {
 public:
  UnableToSendException(const int error_code)
      : ConnectionException(error_code) {}
};

// VALIDATION EXCEPTION

class ValidationException : public std::runtime_error {
 public:
  ValidationException(const std::string input)
      : std::runtime_error(std::format(
            "Illegal address: {}. Should be like 127.0.0.1:80", input)) {}
};

}  // namespace stun
#endif  // STUN_EXCEPTION_H