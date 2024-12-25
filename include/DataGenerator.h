#pragma once
#include <Int.h>
#include <fstream>
#include <functional>
#include <memory>
#include <random>

namespace lingshin {

// 用于生成不同类型的数据
// 统一用同一个接口
class DataGenerator {
protected:
  DataGenerator(int size = 20) : size(size){};

  use Pair = std::pair<int, int>;

public:
  int size = 0;
  virtual ~DataGenerator() = default;
  struct Range : public Pair {
    Range(int a, int b) : Pair(std::min(a, b), std::max(a, b)) {}
  };
  use Function = std::function<int(int)>;
  use Container = std::vector<int>;
  use Unique_ptr = std::unique_ptr<DataGenerator>;
  virtual int operator[](int) = 0;
  static Unique_ptr Default;
  static fn from(Range range, int size = 20) -> Unique_ptr;
  static fn from(int min, int max, int size = 20) -> Unique_ptr {
    return from(Range{min, max}, size);
  };
  static fn from(Function fun, int size = 20) -> Unique_ptr;
  static fn from(Container &&source) -> Unique_ptr;
  static fn from(Container &source) -> Unique_ptr;
  static fn from(String filepath) -> Unique_ptr;
};

class RandomGenerator : public DataGenerator { // 随机数据生成器
  std::uniform_int_distribution<int> dis;
  std::default_random_engine random;

public:
  RandomGenerator(Range range)
    : dis(range.first, range.second), random(std::random_device{}()){};
  int operator[](int index) override { return dis(random); };
};

inline fn DataGenerator::from(Range range, int size) -> Unique_ptr {
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

inline fn DataGenerator::from(Container &&source) -> Unique_ptr {
  return std::make_unique<ArrayGenerator>(std::move(source));
};
inline fn DataGenerator::from(Container &source) -> Unique_ptr {
  return std::make_unique<ArrayGenerator>(source);
}

struct FunctionGenerator : public DataGenerator { // 函数数据生成器
  const Function fun;
  FunctionGenerator(Function fun) : fun(fun){};
  int operator[](int index) override { return fun(index); }
};

inline fn DataGenerator::from(Function fun, int size) -> Unique_ptr {
  var result = std::make_unique<FunctionGenerator>(fun);
  result->size = size;
  return result;
};

struct FileGenerator : public DataGenerator { // 文件数据生成器
  std::ifstream fin;
  FileGenerator(String filepath) : fin(filepath){};
  int operator[](int index) override {
    var result = 0;
    fin >> result;
    return result;
  }
};

inline fn DataGenerator::from(String filepath) -> Unique_ptr {
  return std::make_unique<FileGenerator>(filepath);
};
} // namespace lingshin
