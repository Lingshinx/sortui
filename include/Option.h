#pragma once
#include <Alias.h>
#include <functional>
#include <map>

namespace lingshin {
// 保存用户选项
struct Option {
  float speed = 1;
  enum class Method {
    Bubble,
    Cock,
    Compose,
    Insert,
    Select,
    Quick,
    Merge,
    Heap,
    Shell,
    Bucket,
    Radix,
    Count, // 放在最后用于自动保存排序方法的数量 并非计数排序
  } method = Method::Quick;
  use Task = std::function<void()>;
  // 每次比较交换时进行的操作
  // 因为要通知ui进行刷新所以留了这个选项
  // 可以减少耦合
  // 这在一些地方也叫依赖注入吧
  Task onfresh = [] {};
  use Map = std::map<Method, String>;
  static Map map;
};
} // namespace lingshin
