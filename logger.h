#pragma once

#include <iostream>
#include <source_location>
#include <string_view>

class Log {
 public:
  explicit Log(std::source_location loc = std::source_location::current())
      : functionName_(loc.function_name()) {
    std::cout << indent() << "\\" << functionName_ << "\n";
    ++depth_;
  }

  ~Log() {
    --depth_;
    std::cout << indent() << "/" << functionName_ << "\n";
  }

  Log(const Log&) = delete;
  Log& operator=(const Log&) = delete;

 private:
  static std::string_view indent() {
    static constexpr std::string_view spaces =
        "                                                                ";
    const std::size_t n = static_cast<std::size_t>(depth_) * 2;
    return spaces.substr(0, n < spaces.size() ? n : spaces.size());
  }

  inline static thread_local int depth_ = 0;
  std::string_view functionName_;
};

#define LOG_CONCAT_(a, b) a##b
#define LOG_CONCAT(a, b) LOG_CONCAT_(a, b)
#define LOG() [[maybe_unused]] const Log LOG_CONCAT(logGuard_, __LINE__)
