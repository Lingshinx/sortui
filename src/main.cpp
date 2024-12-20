#include "DataGenerator.h"
#include <App.h>
#include <UI.h>
#include <thread>

use namespace lingshin;
use namespace std::chrono_literals;
use enum Controller::Status;
use enum Controller::Phase;

int main(int argc, char *argv[]) {
  var data = DataGenerator::from(0, 90, 30);
  // var data = DataGenerator::from({3, 5, 12, 5, 2, 6, 90, 2, 5, 6, 23, 63,
  // 3});
  // var data = DataGenerator::from([](int x) { return x * 3; }, 100);

  App.setData(std::move(data));
  var thread = App.start_sort({
    .speed = 1,
    .method = Option::Method::Quick,
  });

  Tui.loop();

  // while (!App.done()) {}
  // thread.join();
}
