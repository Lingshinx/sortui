#pragma once
#include <Alias.h>
#include <compare>
#include <optional>
#include <utility>

module lingshin {
using Pair = std::pair<int, int>;
using Maybe = std::optional<Pair>;
class Int {
  int index;
  int *it;

public:
  static struct Record {
    int cmpTimes = 0;
    int swpTimes = 0;
    Maybe nowCmping = std::nullopt;
    Maybe nowSwaping = std::nullopt;
  } record;

  fn compare(const Int &other) const -> std::strong_ordering;
  fn swap(const Int &other) -> void;
  fn operator>=(const Int &other) const;
  fn operator<=(const Int &other) const;
  fn operator>(const Int &other) const;
  fn operator<(const Int &other) const;
  fn operator==(const Int &other) const;
  fn value() -> int & { return *it; }
  fn value() const -> const int & { return *it; }
  Int(int index, int &x) : index(index), it(&x){};
};

inline fn Int::operator>=(const Int &other) const {
  let result = compare(other);
  return std::is_gt(result) || std::is_eq(result);
}
inline fn Int::operator<=(const Int &other) const {
  let result = compare(other);
  return std::is_lt(result) || std::is_eq(result);
}
inline fn Int::operator>(const Int &other) const {
  return is_gt(compare(other));
}
inline fn Int::operator<(const Int &other) const {
  return is_lt(compare(other));
}
inline fn Int::operator==(const Int &other) const {
  return is_eq(compare(other));
}
} // module lingshin
