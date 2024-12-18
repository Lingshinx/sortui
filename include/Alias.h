#pragma once
#define var auto
#define fn auto
#define let const auto
#define use using
#include <format>
#include <iostream>
#include <string>
template <typename... Args>
consteval void println(const std::string &format_str, Args &&...args) {
  std::string formatted = std::format(format_str, std::forward<Args>(args)...);
  std::cout << formatted << std::endl;
}
