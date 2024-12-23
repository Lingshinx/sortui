#pragma once
#include <Array.h>
#include <DataGenerator.h>
#include <UI.h>
#include <mutex>
#include <ranges>

namespace lingshin {
namespace views = range::views;
namespace time = std::chrono;
extern class Controller {
public:
  Option option;
  enum Status { Comparing, Swapping, NotActive, Sorted } status;
  enum class Phase { StandBy, Ready, Running, Paused, Done };
  std::atomic<Phase> phase;

  // --- 时期控制相关-------------------
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
  fn timePast() -> time::seconds;
  
  // --- 数据控制相关------------------------------
  void setData(DataGenerator::Unique_ptr source);
  fn set_sorted(int index) { isSorted[index] = true; };

  Status get_state_of(int index);
  fn getDataView() { return views::all(data); }
  int getMax() { return max; }
  fn getRecord() -> const Int::Record & { return Int::record; }
  use CallBack = std::function<void(int, int)>;
  void forEach(CallBack);


  static fn getInstance() -> Controller & { return app; };

private:
  int max;

  use TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
  TimePoint start_time;
  TimePoint end_time;
  void done() { phase = Phase::Done; };
  std::mutex statusLock;
  std::condition_variable cond_stop;
  static Controller app;
  Controller() : phase(Phase::StandBy){};

  DataGenerator::Unique_ptr data_generator;
  Array data;
  std::vector<bool> isSorted;
  fn resetRecord(){ Int::record = Int::Record{}; };
  void initData();

} & App;

} // namespace lingshin
