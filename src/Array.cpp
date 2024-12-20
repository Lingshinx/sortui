#include "App.h"
#include <Array.h>
#include <Sort.h>
namespace lingshin {
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

void Array::insert_sort() {
  let length = size();
  for (var index = 1; index < length; ++index) {
    for (var inner = index; inner > 0 && get(inner) < get(inner - 1); --inner) {
      get(inner).swap(get(inner - 1));
    }
  }
}

void Array::select_sort() {
  for (var index = size() - 1; index > 0; --index) {
    var max = get(0);
    for (var inner = 0; inner <= index; ++inner)
      if (var cur = get(inner); cur > max) max = cur;
    max.swap(get(index));
    App.set_sorted(index);
  }
}

void Array::quick_sort() { lingshin::quick_sort(*this, 0, size()); }

void Array::merge_sort() {}
} // namespace lingshin
