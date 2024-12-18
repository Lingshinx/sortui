#pragma once
#include <Int.h>
#include <cstdlib>
#include <functional>
#include <memory>
#include <random>
#include <utility>
#include <vector>
namespace lingshin {
class DataGenerator {
protected:
  DataGenerator(int size = 20) : size(size){};

public:
  int size = 0;
  virtual ~DataGenerator() = default;
  use Range = std::pair<int, int>;
  use Function = std::function<int(int)>;
  use Container = std::vector<int>;
  use Unique_ptr = std::unique_ptr<DataGenerator>;
  virtual int operator[](int) = 0;
  static Unique_ptr from(Range range, int size = 20);
  static Unique_ptr from(int min, int max, int size = 20) {
    return from(Range{min, max}, size);
  };
  static Unique_ptr from(Function fun, int size = 20);
  static Unique_ptr from(Container &&source);
  static Unique_ptr from(Container &source);
};

class RandomGenerator : public DataGenerator { // 随机数据生成器
  std::uniform_int_distribution<int> dis;
  std::default_random_engine random;

public:
  RandomGenerator(Range range)
      : dis(range.first, range.second), random(std::random_device{}()){};
  int operator[](int index) override { return dis(random); };
};

inline DataGenerator::Unique_ptr DataGenerator::from(Range range, int size) {
  var result = std::make_unique<RandomGenerator>(range);
  result->size = size;
  return result;
};

struct ArrayGenerator : public DataGenerator { // 数据数据生成器
  const Container data;
  ArrayGenerator(Container &&source)
      : DataGenerator(source.size()), data(source){};
  ArrayGenerator(Container &source)
      : DataGenerator(source.size()), data(source){};
  int operator[](int index) override { return data.at(index); }
};

inline DataGenerator::Unique_ptr DataGenerator::from(Container &&source) {
  return std::make_unique<ArrayGenerator>(std::move(source));
};
inline DataGenerator::Unique_ptr DataGenerator::from(Container &source) {
  return std::make_unique<ArrayGenerator>(source);
}

struct FunctionGenerator : public DataGenerator { // 函数数据生成器
  const Function fun;
  FunctionGenerator(Function fun) : fun(fun){};
  int operator[](int index) override { return fun(index); }
};

inline DataGenerator::Unique_ptr DataGenerator::from(Function fun, int size) {
  var result = std::make_unique<FunctionGenerator>(fun);
  result->size = size;
  return result;
};
} // namespace lingshin
