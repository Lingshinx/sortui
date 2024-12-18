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
  } method = Method::Quick;
};
} // namespace lingshin
