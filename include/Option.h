#pragma once
namespace lingshin {
struct Option {
  float speed = 1;
  enum class Method {
    Bubble,
    Insert,
    Select,
    Quick,
    Merge,
    Count, // 用于计数
  } method = Method::Quick;
};
} // namespace lingshin
