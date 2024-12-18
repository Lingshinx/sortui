#include <Array.h>
namespace lingshin {

void quick_sort(Array &data, int from, int to) {
  var left = from;
  var right = to;
  while (left < right) {
    do
      --right;
    while (left < right && data[left] < data[right]);
    data[left].swap(data[right]);
    do
      ++left;
    while (left < right && data[right] > data[left]);
    data[left].swap(data[right]);
  }
  quick_sort(data, from, left);
  quick_sort(data, right, to);
}

void merge_sort() {}
} // namespace lingshin
