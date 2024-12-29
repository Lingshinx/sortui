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

// 放心这个函数我也写成友元函数了的
void printSorting(bool &changed) {
  var index = 0;
  let symbols = std::array{"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
  while (true) {
    if (changed) {
      index = (index + 1) % 30;
      std::cout << "\r---排序中" << symbols[index / 3] << "---" << std::flush;
      changed = false;
    } else if (App.isDone())
      break;
  }
}
int main(int argc, char *argv[]) {
  try {
    // 处理命令行参数
    var result = ArguHandler(argc, argv);
    if (result.help) {
      extern const char *HelpMessage;
      std::cout << HelpMessage;
      return 0;
    }
    if (result.datasource) { // 如果选择用文件
      let filepath = *result.datasource;
      App.setData(DataGenerator::from(filepath));
    } else if (result.random) { // 如果选择用随机数
      let[min, max, size] = *result.random;
      App.setData(DataGenerator::from(min, max, size));
    } else if (!result.print) {
      throw std::runtime_error("没有给定数据来源, 请使用\033[32m "
                               "-random\033[0m 或\033[32m -file\033[0m");
    }
    if (result.print) {
      Tui.loop();
    } else {
      var changed = false;
      // 本来可以在onfresh 中输出加载中的动画的
      // 但毕竟输出需要一定时间,
      // 不应该占用宝贵的排序线程
      result.option.onfresh = [&] { changed = true; };
      var sort_thread = App.start_sort(result.option);
      var print_thread = std::thread([&changed] { printSorting(changed); });
      // 等待排序完成
      sort_thread.join();
      print_thread.join();
      // clang-format off
      let &record = App.getRecord();
      std::cout<<std::format( // 用各种颜色打印信息
        "\r---排序完成!---\n"
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
      return 0; // 正常退出
    }
    // 处理运行时错误
  } catch (std::invalid_argument e) {
    // clang-format off
    std::cerr <<
      /* 红色 */ "\033[31m"
      "参数格式错误 "
      /* 无色 */ "\033[0m\""
    << e.what()<< "\"";
    // clang-format on
    return -1;
  } catch (UnknownArgu e) {
    std::cerr << e.getMessage();
    return -1;
  } catch (std::runtime_error e) {
    std::cerr << "\033[31merror:\033[0m  " << e.what();
    return -1;
  } catch (std::bad_alloc e) {
    std::cerr << "缺少需要的参数";
    return -1;
  } catch (std::exception e) {
    std::cerr << "\033[31merror:\033[0m  " << e.what();
    return -1;
  }
}
