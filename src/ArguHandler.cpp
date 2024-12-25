#include <ArguHandler.h>
#include <cstdlib>
#include <format>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
namespace lingshin {
// 将参数用string_view 容器装起来, string_view 只保留引用, 不占太多内存
ArguHandler::Argus::Argus(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    push_back(argv[i]);
  }
}

use namespace std::string_literals;

ArguHandler::ArguHandler(int argc, char *argv[]) {
  var argus = Argus(argc, argv);
  for (var iter = argus.begin(); iter != argus.end(); ++iter) {
    let &eachArgu = *iter; // 给当前处理的参数起一个别名
    // 如果参数不以 dash 开头
    if (eachArgu.front() != '-') throw std::invalid_argument(String(eachArgu));
    let dealError = [eachArgu](std::string_view argu) {
      if (eachArgu.size() > 2 && eachArgu != argu)
        throw UnknownArgu(eachArgu, argu);
    };
    switch (eachArgu[1]) { // 排除了dash 符号后的第一个字母
    case 'h':
      dealError("-help");
      help = true;
      break;
    case 'n':
      dealError("-no-ui");
      print = false;
      break;
    case 'f':
      dealError("-file");
      ++iter;
      datasource = *iter;
      break;
    case 'm':
      dealError("-method");
      option.method = switchMethod(*++iter);
      break;
    case 'r':
      dealError("-random");
      try {
        // don't repeat yourself 不要重复你自己
        let getNum = [&iter]() { return std::stoi(String(*++iter)); };
        let min = getNum();
        let max = getNum();
        let size = getNum();
        random = std::make_tuple(min, max, size);
      } catch (std::exception e) {
        throw std::runtime_error(
          "缺少足够的参数生成随机数, 请依次指定最小值, 最大值, 和长度");
      }
      break;
    case 's':
      dealError("-speed");
      option.speed = std::stof(String(*++iter));
      break;
    default: throw std::invalid_argument(String(eachArgu));
    }
  }
}

// clang-format off
const  String UnknownArgu::getMessage() const noexcept {
  return std::format(
    "不认识的参数"
    "\033[31m {0} \033[0m"
    "你是想说"
    "\033[32m {1} \033[0m"
    "吗",
    wrong, right
  );
}
} // namespace lingshin
