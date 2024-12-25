#pragma once
#include <Alias.h>
#include <algorithm>
#include <compare>
#include <optional>
#include <utility>

namespace lingshin {
use Pair = std::pair<int, int>;
use Maybe = std::optional<Pair>;
// 包装int, 用于实时记录比较次数
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
    // use space
    fn usepace(int size) {
      _spaceUsed += size;
      spaceUsed = std::max(spaceUsed, _spaceUsed); // 只记录最大空间占用
    };

  private:
    int _spaceUsed = 0;
  } record;

  Int(int index, int &x) : index(index), it(&x){};

  // 重载运算符, 让我更优雅地写各个排序算法
  // 这个时候就可以批评一下java了
  // 从c++ 抄这抄那的, 反而面向对象中相对重要的运算符重载不抄
  // 一个好的类型是可以做到像基础类型一样运用自然的
  // 而java 永远做不到
  fn compare(const Int &other) const -> std::strong_ordering;
  fn compare(int other) const -> std::strong_ordering;
  fn swap(const Int &other) -> void;
  template <typename Type> // 为什么不能批量地模板化
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
