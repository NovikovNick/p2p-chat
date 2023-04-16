#ifndef UTIL_LOGGER_H
#define UTIL_LOGGER_H
#include <format>
#include <iostream>

namespace util {

template <typename... Args>
void log(const std::string_view& str, Args&&... args) {
  std::cout << std::vformat(str, std::make_format_args(args...));
}
}  // namespace util
#endif  // UTIL_LOGGER_H