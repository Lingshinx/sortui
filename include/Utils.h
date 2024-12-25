#include <Array.h>
namespace lingshin {
class Utils {
  // 出于老师的要求
  // 说不允许出现类以外的函数
  // 虽然难以理解，我也只好照办
  friend int maxDigit(int num);
  friend void shell_insert_sort(Array &data, int from, int step);
  friend void adjust(Array &data, int index, int last);
  friend void merge_sort(Array &data, int from, int to);
  friend void merge(Array &data, int from, int mid, int to);
  friend void quick_sort(Array &data, int from, int to);
};
} // namespace lingshin
