#include <ArguHandler.h>
#include <cstdlib>
#include <format>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
namespace lingshin {
ArguHandler::Argus::Argus(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    push_back(argv[i]);
  }
}

use namespace std::string_literals;

ArguHandler::ArguHandler(int argc, char *argv[]) {
  var argus = Argus(argc, argv);
  for (var iter = argus.begin(); iter != argus.end(); ++iter) {
    let &eachArgu = *iter;
    if (eachArgu.front() != '-') throw std::invalid_argument(String(eachArgu));
    let dealError = [eachArgu](std::string_view argu) {
      if (eachArgu.size() > 2 && eachArgu != argu)
        throw UnknownArgu(eachArgu, argu);
    };
    switch (eachArgu[1]) {
    case 'n':
      dealError("-no-print");
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
      {
        let getNum = [&iter]() { return std::stoi(String(*++iter)); };
        let min = getNum();
        let max = getNum();
        let size = getNum();
        random = std::make_tuple(min, max, size);
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
