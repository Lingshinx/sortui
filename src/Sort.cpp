#include <Array.h>
namespace lingshin {

void quick_sort(Array &data, int from, int to) {
  if (to - from <= 1) return;
  int left = from;
  int right = to - 1;
  var pivot = data[from];

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

  quick_sort(data, from, right + 1);
  quick_sort(data, left, to);
}

void merge_sort() {}

} // namespace lingshin
