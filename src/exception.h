#ifndef CHAT_EXCEPTION_H
#define CHAT_EXCEPTION_H
#include <format>
#include <stdexcept>

namespace chat {

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

class StunServerUnavalableException : public std::runtime_error {
 public:
  StunServerUnavalableException() : std::runtime_error("") {}
};

class StunResponseParseException : public std::runtime_error {
 public:
  StunResponseParseException() : std::runtime_error("") {}
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

}  // namespace chat
#endif  // CHAT_EXCEPTION_H