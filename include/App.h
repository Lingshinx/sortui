#pragma once
#include <Array.h>
#include <DataGenerator.h>
#include <UI/UI.h>
#include <mutex>
#include <ranges>

namespace lingshin {
namespace views = range::views;
namespace time = std::chrono;

// 总控全局的类，用于生成数据，管理数据，获取信息，控制排序流程等
extern class Controller {
public:
  Option option;
  // StandBy 是待机状态, 指还没有输入数据的状态
  enum class Phase { StandBy, Ready, Running, Paused, Done };
  enum Status { Comparing, Swapping, NotActive, Sorted } status;
  std::atomic<Phase> phase;

  // --- 时期控制相关-------------------
  fn start_sort() -> std::thread;
  fn start_sort(Option option) {
    this->option = option;
    return start_sort();
  };
  // 对应ui 中的空格键操作
  // 没开始就开始, 运行中就暂停, 暂停中就开始, 结束了就重开
  void toggle();
  void wait();
  void pause();
  // 重新启动
  void resume();
  bool isDone() { return phase == Phase::Done; }
  bool isIdle() {
    use enum Phase;
    return phase == StandBy || phase == Ready || phase == Done;
  }
  // 返回排序花费的时间
  fn timePast() -> time::seconds;

  // --- 数据控制相关------------------------------
  void setData(DataGenerator::Unique_ptr source);
  fn set_sorted(int index) { isSorted[index] = true; };

  Status get_state_of(int index);
  fn getDataView() { return views::all(data); }
  // 这个不是给排序的时候用的, 只是方便UI 展示
  // 总不可能数据有多大, 那个条就有多长吧
  // 我是按 屏幕长度* value / 最大值来展示的
  // 我觉得获取最大值也应该包含在排序用时当中
  int getMax() { return max; }
  fn getRecord() -> const Int::Record & { return Int::record; }
  use CallBack = std::function<void(int, int)>;
  void forEach(CallBack);

  static fn getInstance() -> Controller & { return app; };

private:
  // 关于我为什么喜欢写类型别名和自动类型, 你看看这个类型有多长
  // 如果你的变量名足够有描述性, 类型反而不很重要
  // 说到底类型是服务于编译器, 而不是人的
  // 不过我还是更喜欢静态类型
  use TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
  TimePoint start_time;
  TimePoint end_time;
  void done() { phase = Phase::Done; };
  std::mutex statusLock;
  std::condition_variable cond_stop;
  static Controller app;
  Controller() : phase(Phase::StandBy){};

  int max;
  DataGenerator::Unique_ptr data_generator;
  Array data;
  std::vector<bool> isSorted;
  fn resetRecord() { Int::record = Int::Record{}; };
  void initData();

} & App;

} // namespace lingshin
