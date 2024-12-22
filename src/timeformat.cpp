#include <Alias.h>
#include <Tools.h>
#include <chrono>
#include <format>

namespace lingshin::tool {
namespace time = std::chrono;

fn formatDuration(time::seconds duration) -> String {
  // 获取小时、分钟和秒
  let hours = cast<time::hours>(duration);
  duration -= hours;
  let minutes = cast<time::minutes>(duration);
  duration -= minutes;
  let seconds = cast<time::seconds>(duration);

  // 使用 std::format 格式化输出
  if (hours.count() > 0) {
    return std::format(
      "{:2}h{:2}m{:2}s", hours.count(), minutes.count(), seconds.count());
  } else if (minutes.count() > 0) {
    return std::format("{:2}m{:2}s", minutes.count(), seconds.count());
  } else {
    return std::format("{:2}s", seconds.count());
  }
}
} // namespace lingshin::tool
