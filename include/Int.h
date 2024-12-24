#pragma once
#include <Alias.h>
#include <algorithm>
#include <compare>
#include <optional>
#include <utility>

namespace lingshin {
use Pair = std::pair<int, int>;
use Maybe = std::optional<Pair>;
class Int {
  int index;
  int *it;

public:
  static struct Record {
    int cmpTimes = 0;
    int swpTimes = 0;
    int spaceUsed = 0;
    Maybe nowCmping = std::nullopt;
    Maybe nowSwaping = std::nullopt;

    fn free(int size) { _spaceUsed -= size; };
    fn usepace(int size) {
      _spaceUsed += size;
      spaceUsed = std::max(spaceUsed, _spaceUsed);
    };

  private:
    int _spaceUsed = 0;
  } record;

  Int(int index, int &x) : index(index), it(&x){};

  fn compare(const Int &other) const -> std::strong_ordering;
  fn compare(int other) const -> std::strong_ordering;
  fn swap(const Int &other) -> void;
  template <typename Type>
  fn operator>=(Type other) const;
  template <typename Type>
  fn operator<=(Type other) const;
  template <typename Type>
  fn operator>(Type other) const;
  template <typename Type>
  fn operator<(Type other) const;
  template <typename Type>
  fn operator==(Type other) const;
  fn value() -> int & { return *it; }
  fn value() const -> const int & { return *it; }
  operator int() { return *it; }
};

template <typename Type>
inline fn Int::operator>=(Type other) const {
  let result = compare(other);
  return std::is_gt(result) || std::is_eq(result);
}
template <typename Type>
inline fn Int::operator<=(Type other) const {
  let result = compare(other);
  return std::is_lt(result) || std::is_eq(result);
}
template <typename Type>
inline fn Int::operator>(Type other) const {
  return is_gt(compare(other));
}
template <typename Type>
inline fn Int::operator<(Type other) const {
  return is_lt(compare(other));
}
template <typename Type>
inline fn Int::operator==(Type other) const {
  return is_eq(compare(other));
}
} // namespace lingshin
