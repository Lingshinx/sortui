#pragma once
#include <Alias.h>
#include <map>

namespace lingshin {
struct Option {
  float speed = 1;
  enum class Method {
    Bubble,
    Insert,
    Select,
    Quick,
    Merge,
    Heap,
    Shell,
    Bucket,
    Radix ,
    Count, // 用于计数
  } method = Method::Quick;
  use Map = std::map<Method, String>;
  static Map map;
};
} // namespace lingshin
