#pragma once
#include <Array.h>
#include <DataGenerator.h>
#include <Option.h>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <ranges>
#include <thread>
#include <unistd.h>

namespace lingshin {
namespace views = std::ranges::views;
extern class Controller {
public:
  Option option;
  enum class Phase { Running, Paused, StandBy, Done };
  enum Status { Comparing, Swapping, NotActive } status;

  std::atomic<Phase> phase;

  void setData(DataGenerator &source);
  void setData(DataGenerator::Unique_ptr source) { setData(*source); };

  fn getDataView() { return views::all(data); }

  fn start_sort() -> std::thread;
  fn start_sort(Option option) {
    this->option = option;
    return start_sort();
  };

  void wait();
  void pause();
  void resume();
  bool done() { return phase == Phase::Done; }

  Status get_state_of(int index);
  int getMax() { return max; }

  use CallBack = std::function<void(int, int)>;
  void forEach(CallBack);

  fn getRecord() -> const Int::Record & { return Int::record; }

  static fn getInstance() -> Controller & { return app; };

private:
  int max;
  std::mutex statusLock;
  std::condition_variable cond_stop;
  static Controller app;
  Controller() { phase = Phase::StandBy; };
  Array data;
} & App;

} // namespace lingshin
