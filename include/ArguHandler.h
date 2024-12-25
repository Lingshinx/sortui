#include "Option.h"
#include <Alias.h>
#include <cctype>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <vector>
namespace lingshin {
class UnknownArgu : public std::exception {
  const String wrong;
  const String right;

public:
  UnknownArgu(std::string_view message, std::string_view right)
    : wrong(message), right(right) {}
  const String getMessage() const noexcept;
};
class ArguHandler {
  use File = std::optional<String>;
  use Random = std::optional<std::tuple<int, int, int>>;

  struct Argus : std::vector<std::string_view> {
    Argus(int argc, char *argv[]);
  };

  Option::Method switchMethod(std::string_view method) {
    use enum Option::Method;
    switch (tolower(method.front())) {
    case 'i': return Insert;
    case 'q': return Quick;
    case 'h': return Heap;
    case 'r': return Radix;
    case 'm': return Merge;
    case 's': return method[1] == 'h' ? Shell : Select;
    case 'b': return method[2] == 'c' ? Bucket : Bubble;
    }
    throw std::invalid_argument("不认识的排序方法");
  }

public:
  File datasource = std::nullopt;
  Random random = std::nullopt;
  Option option;
  bool print = true;
  ArguHandler(int argc, char *argv[]);
};
} // namespace lingshin
