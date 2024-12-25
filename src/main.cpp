#include "App.h"
#include "DataGenerator.h"
#include "Option.h"
#include "Tools.h"
#include <ArguHandler.h>
#include <UI/UI.h>
#include <array>
#include <iostream>
#include <new>
#include <stdexcept>
#include <thread>

use namespace lingshin;
use namespace std::chrono_literals;

int main(int argc, char *argv[]) {
  try {
    var result = ArguHandler(argc, argv);
    if (result.datasource) {
      let filepath = *result.datasource;
      App.setData(DataGenerator::from(filepath));
    }
    if (result.random) {
      let[min, max, size] = *result.random;
      App.setData(DataGenerator::from(min, max, size));
    }
    if (result.print) {
      Tui.loop();
    } else {
      var changed = false;
      result.option.onfresh = [&] { changed = true; };
      var thread = App.start_sort(result.option);
      var index = 0;
      let symbols =
        std::array{"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
      while (true) {
        if (changed) {
          index = (index + 1) % 30;
          std::cout << "\r---排序中" << symbols[index / 3] << "---"
                    << std::flush;
          changed = false;
        } else if (App.isDone())
          break;
      }
      thread.join();
      // clang-format off
      let &record = App.getRecord();
      std::cout<<std::format("\r---排序完成!---\n"
        "\033[1;32m数据长度:\033[0m {}\n"
        "\033[1;33m排序方法:\033[0m {}\n"
        "\033[1;34m花费时间:\033[0m {}\n"
        "\033[1;35m比较次数:\033[0m {}\n"
        "\033[1;36m交换次数:\033[0m {}\n"
        "\033[1;31m空间占用:\033[0m {}\n",
        App.getDataView().size(),
        Option::map[App.option.method],
        tool::formatDuration(App.timePast()),
        record.cmpTimes,
        record.swpTimes,
        record.spaceUsed
      );
      // clang-format on
    }
  } catch (std::invalid_argument e) {
    // clang-format off
    std::cerr <<
      /* 红色 */ "\033[31m"
      "参数格式错误 "
      /* 无色 */ "\033[0m\""
    << e.what()<< "\"";
    // clang-format on
  } catch (UnknownArgu e) {
    std::cerr << e.getMessage();
  } catch (std::runtime_error e) {
    std::cerr << "\033[31merror:\033[0m  " << e.what();
  } catch (std::bad_alloc e) {
    std::cerr << "缺少需要的参数";
  } catch (std::exception e) {
    std::cerr << "\033[31merror:\033[0m  " << e.what();
  }
}
