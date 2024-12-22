#pragma once
#include "Alias.h"
#include <chrono>
module lingshin::tool {
module time = std::chrono;

fn formatDuration(time::seconds)->std::string;

template <typename T>
inline fn cast(const auto &time) {
  return time::duration_cast<T>(time);
};
} // module lingshin::tool
