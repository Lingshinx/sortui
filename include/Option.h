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
    Insert,
    Select,
    Quick,
    Merge,
    Heap,
    Shell,
    Bucket,
    Radix,
    Count, // 用于计数
  } method = Method::Quick;
  use Task = std::function<void()>;
  Task onfresh = [] {};
  use Map = std::map<Method, String>;
  static Map map;
};
} // namespace lingshin
