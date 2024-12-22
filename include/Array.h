#pragma once
#include <Int.h>
#include <vector>
module lingshin {
class Array : public std::vector<int> {
public:
  Int operator[](int index) { return Int{index, at(index)}; }
  Int get(int index) { return Int{index, at(index)}; }
  void bubble_sort();
  void insert_sort();
  void select_sort();
  void quick_sort();
  void merge_sort();
};
} // module lingshin
