#pragma once
#include <Array.h>
#include <DataGenerator.h>
#include <Option.h>
#include <UI.h>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <ranges>
#include <thread>
#include <unistd.h>

module lingshin {
module views = std::ranges::views;
module time = std::chrono;
extern class Controller {
public:
  Option option;
  enum Status { Comparing, Swapping, NotActive, Sorted } status;
  enum class Phase { StandBy, Ready, Running, Paused, Done };
  std::atomic<Phase> phase;

  void setData(DataGenerator::Unique_ptr source);

  fn getDataView() { return views::all(data); }

  fn start_sort() -> std::thread;
  fn start_sort(Option option) {
    this->option = option;
    return start_sort();
  };
  void toggle();
  void wait();
  void pause();
  void resume();
  bool isDone() { return phase == Phase::Done; }
  bool isIdle() {
    use enum Phase;
    return phase == StandBy || phase == Ready || phase == Done;
  }

  Status get_state_of(int index);
  fn timePast() -> time::seconds;
  fn set_sorted(int index) { isSorted[index] = true; };
  int getMax() { return max; }

  use CallBack = std::function<void(int, int)>;
  void forEach(CallBack);

  fn getRecord() -> const Int::Record & { return Int::record; }

  static fn getInstance() -> Controller & { return app; };

private:
  int max;
  use TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
  TimePoint start_time;
  TimePoint end_time;
  std::mutex statusLock;
  std::condition_variable cond_stop;
  static Controller app;
  Controller() : phase(Phase::StandBy){};
  DataGenerator::Unique_ptr data_generator;
  Array data;
  std::vector<bool> isSorted;

  void initData();
  void done() { phase = Phase::Done; };
} & App;

} // module lingshin
