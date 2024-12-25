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
  for (let index : iota(0, last_unadjusted) | reverse) // 建堆
    adjust(data, index, lastIndex);
  for (let last : iota(0, lastIndex + 1) | reverse) { // 排序
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

int maxDigit(int num) {
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
  int digit = 0;
  int getDigit() { return digit; };
  use Container = SpaceRecorder<std::list<int>>;
  std::array<Container, 0x10> radixes;
  void next_digit() { digit += 4; }
  int digit_of(int num) { return num >> digit & 0xf; }
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

template <typename Type>
int &SpaceRecorder<Type>::spaceUsed = Int::record.spaceUsed;

template <typename Type>
Int::Record &SpaceRecorder<Type>::record = Int::record;
} // namespace lingshin
