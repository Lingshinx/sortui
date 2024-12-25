#pragma once
#include "Alias.h"
#include <chrono>
namespace lingshin::tool {

namespace time = std::chrono;
class TimeUtil { // 形式主义地放一下
  friend fn formatDuration(time::seconds)->std::string;
  friend fn cast(const auto &time);
};

// 用于格式化时间 格式为${d}h${d}m${d}s的格式
fn formatDuration(time::seconds)->std::string;
template <typename T>
inline fn cast(const auto &time) {
  return time::duration_cast<T>(time);
};
} // namespace lingshin::tool
