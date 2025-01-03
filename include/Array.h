#pragma once
#include <Int.h>
#include <list>
#include <queue>
#include <vector>

namespace lingshin {

// 对容器进行简单地包装，以便实时记录空间占用
template <typename Type>
class SpaceRecorder : public Type {
  static int &spaceUsed;
  static Int::Record &record;

public:
  use Base = Type;
  SpaceRecorder(int size = 0) : Base(size) { record.usepace(size); }
  ~SpaceRecorder() { record.free(Base::size()); }
  void push_back(int value) {
    Base::push_back(value);
    record.usepace(1);
  };
  void resize(int new_size) {
    record.usepace(new_size - Base::size());
    Base::resize(new_size);
  };
  // 因为各个窗口的迭代器类型略有不同所以用了自动参数
  void insert(auto pos, int value) {
    record.usepace(1);
    Base::insert(pos, value);
  }
};

class Array : public std::vector<int> {
  double sortedness();

public:
  // 给外部使用的运算符
  Int operator[](int index) { return Int{index, at(index)}; }
  // 给内部方法使用的方法
  Int get(int index) { return Int{index, at(index)}; }
  Array(int size) : std::vector<int>(size) {};
  Array() : std::vector<int>() {};

  static fn new_vec(int size = 0) {
    return SpaceRecorder<std::vector<int>>(size);
  };

  static fn new_que(int size = 0) {
    return SpaceRecorder<std::deque<int>>(size);
  }

  static fn new_list(int size = 0) {
    return SpaceRecorder<std::list<int>>(size);
  }

  void bubble_sort();
  void cock_sort();
  void insert_sort();
  void select_sort();
  void quick_sort();
  void merge_sort();
  void heap_sort();
  void shell_sort();
  void bucket_sort();
  void radix_sort();
  void compose_sort();
};
} // namespace lingshin
