#include <Alias.h>
#include <chrono>
#include <format>

namespace lingshin::tool {
namespace time = std::chrono;
template <typename T>
inline fn cast(const auto &time) {
  return time::duration_cast<T>(time);
};

fn formatDuration(time::seconds duration) {
  // 获取小时、分钟和秒
  var hours = cast<time::hours>(duration);
  duration -= hours;
  var minutes = cast<time::minutes>(duration);
  duration -= minutes;
  var seconds = cast<time::seconds>(duration);

  // 使用 std::format 格式化输出
  if (hours.count() > 0) {
    return std::format(
      "{:02}h{:02}m{:02}s", hours.count(), minutes.count(), seconds.count());
  } else if (minutes.count() > 0) {
    return std::format("{:02}m{:02}s", minutes.count(), seconds.count());
  } else {
    return std::format("{:02}s", seconds.count());
  }
}
} // namespace lingshin::tool
