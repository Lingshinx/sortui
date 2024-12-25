#include "App.h"
#include <Array.h>
#include <algorithm>
#include <list>
#include <ranges>

namespace lingshin {
//---冒泡排序----------
void Array::bubble_sort() {
  for (var index = size() - 1; index > 0; --index) {
    for (var inner = 0; inner < index; ++inner) {
      var cur = get(inner);
      var after = get(inner + 1);
      if (cur > after) cur.swap(after);
    }
    App.set_sorted(index);
  }
}

// --- 鸡尾酒排序-------------
void Array::cock_sort() {
  var swapped = true;
  var left = 0;
  var right = size() - 1;
  var index = left;
  // 因为不允许我写类以外的函数
  // 那我只好写局部匿名函数了
  let judgeAndSwap = [&](int a, int b) {
    var _a = get(a), _b = get(b);
    if (_a > _b) {
      swapped = true;
      _a.swap(_b);
    }
  };
  while (swapped) {
    swapped = false;
    while (++index < right)
      judgeAndSwap(index - 1, index);
    App.set_sorted(right);
    --right;
    if (!swapped) break; // 提前结束
    while (--index > left)
      judgeAndSwap(index, index + 1);
    App.set_sorted(left);
    ++left;
  }
};

//----插入排序--------------------------
void Array::insert_sort() {
  let length = size();
  for (var index = 1; index < length; ++index) {
    for (var inner = index; inner > 0 && get(inner) < get(inner - 1); --inner) {
      get(inner).swap(get(inner - 1));
    }
  }
}

//----选择排序--------------------------
void Array::select_sort() {
  for (var index = size() - 1; index > 0; --index) {
    var max = get(0);
    for (var inner = 0; inner <= index; ++inner)
      if (var cur = get(inner); cur > max) max = cur;
    max.swap(get(index));
    App.set_sorted(index);
  }
}

//----快速排序--------------------------
void quick_sort(Array &data, int from, int to) {
  if (to - from <= 1) {
    App.set_sorted(from);
    return;
  };
  int left = from;
  int right = to - 1;
  let pivot = data[from];

  while (left <= right) {
    while (left <= right && data[left] < pivot)
      ++left;
    while (left <= right && data[right] > pivot)
      --right;
    if (left <= right) {
      data[left].swap(data[right]);
      ++left;
      --right;
    }
  }

  App.set_sorted(right);

  quick_sort(data, from, right + 1);
  quick_sort(data, left, to);
}

void Array::quick_sort() { lingshin::quick_sort(*this, 0, size()); }

//----归并排序--------------------------
void merge(Array &data, int from, int mid, int to) {
  int size = to - from;
  var temp = Array::new_que();
  var a = from, b = mid;
  while (true) {
    if (a >= mid && b >= to) break;
    if (a >= mid) {
      temp.push_back(data[b++]);
    } else if (b >= to) {
      temp.push_back(data[a++]);
    } else {
      temp.push_back(data[a] < data[b] ? data[a++] : data[b++]);
    }
  }
  var index = from;
  for (let it : temp) {
    data.at(index++) = it;
    App.wait();
  }
};

void merge_sort(Array &data, int from, int to) {
  if (to - from <= 1) return;
  let mid = from + (to - from) / 2;
  merge_sort(data, from, mid);
  merge_sort(data, mid, to);
  merge(data, from, mid, to);
}

void Array::merge_sort() { lingshin::merge_sort(*this, 0, size()); }

// ---堆排序--------------------
struct node {
  int index;
  node(int x) : index(x){};
  operator int() { return index; }
  // 左右子堆
  fn left() const { return 2 * index + 1; }
  fn right() const { return 2 * index + 2; }
};

void adjust(Array &data, int index, int last) {
  let end_node = last - 1;
  let current = node(index);
  let left = current.left();
  let right = current.right();
  let cur = current.index;
  if (current.left() == end_node) { // 只有子节点的情况
    if (data[cur] < data[left]) data[cur].swap(data[left]);
  } else if (right <= end_node) { // 子女双全的情况
    let leftBigger = data[left] > data[cur];
    let rightBigger = data[right] > data[cur];
    var which = 0;
    if (leftBigger && rightBigger) {
      which = data[left] > data[right] ? left : right;
    } else if (leftBigger) {
      which = left;
    } else if (rightBigger) {
      which = right;
    } else {
      return;
    }
    data[cur].swap(data[which]);
    adjust(data, which, last);
  }
}

void Array::heap_sort() {
  use namespace view;
  int lastIndex = size() - 1;
  int last_unadjusted = (lastIndex) / 2;
  var &data = *this;
  // view 是惰性求值的
  // 我十分青睐于函数式编程
  // 最近在学 Haskell
  for (let index : iota(0) | take(last_unadjusted) | reverse) // 建堆
    adjust(data, index, lastIndex);
  for (let last : iota(0) | take(lastIndex + 1) | reverse) { // 排序
    get(0).swap(get(last));
    App.set_sorted(last);
    adjust(data, 0, last);
  }
};

// ---希儿排序------------------------------
void shell_insert_sort(Array &data, int from, int step) {
  let length = data.size();
  for (var index = from; index < length; index += step)
    for (var inner = index; inner >= step && data[inner] < data[inner - step];
         inner -= step)
      data[inner].swap(data[inner - step]);
}

int maxDigit(int num) { // 返回最大的二进制非零位
  if (num <= 1) return num;
  return maxDigit(num >> 1) << 1;
}

void Array::shell_sort() {
  use namespace view;
  var length = size();
  var &data = *this;
  for (var step = maxDigit(length); step > 0; step >>= 1)
    for (let eachGroup : iota(0, step))
      shell_insert_sort(data, eachGroup, step);
}

// --- 桶排序------------------
struct Buckets {
  int max, min, step;
  struct Bucket : public SpaceRecorder<std::list<int>> {
    use Base = SpaceRecorder<std::list<int>>;
    void insert(Int value) {
      let it = range::find_if(*this, [=](int x) { return value > x; });
      Base::insert(it, value);
    }
  };
  std::vector<Bucket> buckets;
  Buckets(int max, int min, int step)
    : max(max), min(min), step(step), buckets((max - min) / step + 1) {}
  void push(Int value) {
    let index = (value - min) / step;
    buckets[index].insert(value);
  };

  fn collection() { return buckets | view::join; }
};

void Array::bucket_sort() {
  var &data = *this;
  let max = range::max(data);
  let min = range::min(data);
  let sqroot = static_cast<int>(sqrt(size()));
  let step = (max - min) / sqroot;
  var buckets = Buckets(max, min, step);
  for (let index : view::iota(0) | view::take(size())) {
    buckets.push(data[index]);
    let view = buckets.collection();
    range::copy(view, data.begin());
  }
}

// --- 基数排序 -------------------
struct Radix {
  // 有些基数排序是用十进制进行的
  // 但我认为这并不合理,
  // cpu 进行位运算应该更快一些
  int digit = 0;
  int getDigit() { return digit; };
  use Container = SpaceRecorder<std::list<int>>;
  std::array<Container, 0x10> radixes;
  void next_digit() { digit += 4; }
  int digit_of(int num) { return num >> digit & 0xf; } // 获取对应位的大小
  void push(int num) { radixes[digit_of(num)].push_back(num); }
  fn view() { return radixes | view::join; };
  fn collect() {
    var result = Container{};
    for (var &each : radixes)
      result.splice(result.end(), each);
    return result;
  };
};

void Array::radix_sort() {
  var radixes = Radix{};
  var &data = *this;
  for (let it : *this)
    radixes.push(it);
  let max = range::max(data);
  for (let max_digit = maxDigit(max); 1 << radixes.getDigit() <= max_digit;
       radixes.next_digit()) {
    let collection = radixes.collect();
    for (let it : collection)
      radixes.push(it);
    range::copy(radixes.view(), data.begin());
    App.wait();
  }
}

double Array::sortedness() {
  var inverted = 0;
  int length = size();
  for (let index : view::iota(0, length - 1))
    for (let after : view::iota(index + 1, length))
      if (get(index) > get(after)) ++inverted;
  int max_inversions = length * (length - 1) / 2;
  // 有序性 = 1 - (逆序对数 / 最大逆序对数)
  return 1.0 - static_cast<double>(inverted) / max_inversions;
  return inverted;
}

void Array::compose_sort() {
  let length = size();
  if (length < 20) {
    cock_sort(); // 数据量小时用什么都差不多
  } else if (length < 1000) {
    if (sortedness() < 0.4) // 很混乱时 用快排
      quick_sort();
    else if (sortedness() < 0.9)
      shell_sort();
    else // 如果比较有序, 用插入排序
      insert_sort();
  } else {
    if (range::max(*this) > 10000)
      merge_sort(); // 当数据量很大时使用归并排序
    else
      radix_sort(); // 数据的位数很小时, 基数排序
  }
}

template <typename Type>
int &SpaceRecorder<Type>::spaceUsed = Int::record.spaceUsed;

template <typename Type>
Int::Record &SpaceRecorder<Type>::record = Int::record;
} // namespace lingshin
