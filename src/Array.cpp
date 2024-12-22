#include "App.h"
#include <Array.h>
#include <Sort.h>
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
  var temp = Array{};
  temp.reserve(size);
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
void adjust(Array &data, int index) {
  let size = data.size();
  if (index * 2 > size) {
  }
}

void Array::heap_sort() {};
//
} // namespace lingshin
