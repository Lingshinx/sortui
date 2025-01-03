#include <App.h>
#include <Int.h>
namespace lingshin {
Int::Record Int::record;
fn Int::compare(const Int &other) const -> std::strong_ordering {
  ++record.cmpTimes;
  record.nowCmping = Pair{index, other.index};
  App.wait(); // 这就是我变速的密诀
  record.nowCmping = std::nullopt;
  return value() <=> other.value();
}
fn Int::compare(int other) const -> std::strong_ordering {
  ++record.cmpTimes;
  record.nowCmping = {index, -1}; // 写成-1 就不会打印在屏幕了
  App.wait();
  record.nowCmping = std::nullopt;
  return other <=> *it;
}
fn Int::swap(const Int &other) -> void {
  ++record.swpTimes;
  record.nowSwaping = Pair{index, other.index};
  App.wait();
  std::swap(*it, *other.it);
  record.nowSwaping = std::nullopt;
}
} // namespace lingshin
