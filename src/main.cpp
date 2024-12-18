#include <App.h>
#include <format>

use namespace lingshin;
use namespace std::chrono_literals;
use enum Controller::Status;
use enum Controller::Phase;

int main(int argc, char *argv[]) {
  var data = DataGenerator::from(0, 9);

  App.setData(std::move(data));
  var thread = App.start_sort({
      .speed = 2,
      .method = Option::Method::Bubble,
  });

  while (!App.done()) {
    std::cout << "\033c";
    App.forEach([](int index, int value) {
      var it = App.get_state_of(index);
      switch (it) {
      case Comparing:
        std::cout << "\033[32m";
        break;
      case Swapping:
        std::cout << "\033[31m";
        break;
      case NotActive:
        break;
      }
      std::cout << value << " \033[0m";
    });
    std::cout << std::endl;
    App.wait();
  }
}
