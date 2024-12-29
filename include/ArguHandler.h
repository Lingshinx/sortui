#include "Option.h"
#include <Alias.h>
#include <cctype>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <vector>
namespace lingshin {
// 我自定义的异常类
// 一开始没有想要错误处理
// 感觉很多错误都是可以通过逻辑避免的
// 所以只在处理用户参数的时候用了
class UnknownArgu : public std::exception {
  const String wrong;
  const String right;

public:
  UnknownArgu(std::string_view message, std::string_view right)
    : wrong(message), right(right) {}
  const String getMessage() const noexcept;
};

//
class ArguHandler {
  use File = std::optional<String>;
  use Random = std::optional<std::tuple<int, int, int>>;
  // 将参数用string_view 容器装起来, string_view 只保留引用, 不占太多内存
  struct Argus : std::vector<std::string_view> {
    Argus(int argc, char *argv[]);
  };

  fn switchMethod(std::string_view method) -> Option::Method;

public:
  File datasource = std::nullopt;
  Random random = std::nullopt;
  Option option; // 最后要传给App的option
  bool print = true;
  bool help = false;
  ArguHandler(int argc, char *argv[]);
};
} // namespace lingshin
