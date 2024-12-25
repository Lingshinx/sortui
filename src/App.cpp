#include "Option.h"
#include <App.h>
#include <Int.h>
#include <Tools.h>
#include <algorithm>
#include <chrono>
#include <exception>
#include <mutex>
#include <thread>

namespace lingshin {
using namespace std::chrono_literals;

Controller Controller::app;
Controller &App = Controller::getInstance(); // 全局静态单例变量，思路不错吧

static bool in(Pair pair, int index) {
  return index == pair.first || index == pair.second;
}

void Controller::initData() {
  var &source = *data_generator;
  var index = 0;
  data.resize(source.size);
  isSorted.resize(source.size);
  for (var &it : data)
    it = source[index++];

  // 因为vector<bool>其实是类似于bitset的结构, 所以不能用range::fill, 可惜
  std::fill(isSorted.begin(), isSorted.end(), false);
  max = range::max(data);
  phase = Phase::Ready;
}

Controller::Status Controller::get_state_of(int index) {
  use enum Status;
  let &pair = getRecord();
  if (isSorted[index]) return Sorted;
  if (let now = pair.nowCmping; now && in(*now, index)) return Comparing;
  if (let now = pair.nowSwaping; now && in(*now, index)) return Swapping;
  return NotActive;
};

void Controller::forEach(CallBack callback) {
  for (int index = 0; index < data.size(); ++index)
    callback(index, data.at(index));
}

fn Controller::timePast() -> time::seconds {
  if (phase == Phase::Ready) return 0s;
  let now = phase == Phase::Done ? end_time : time::steady_clock::now();
  return tool::cast<time::seconds>(now - start_time);
}

void Controller::wait() {
  use enum Phase;
  var lock = std::unique_lock<std::mutex>{statusLock};
  if (option.speed < 0.01) phase = Paused;
  if (phase == Paused) cond_stop.wait(lock, [&] { return phase != Paused; });
  // 我觉得这个挺巧妙的，不知道事实上的倍速是不是同原理
  let duration = 100ms / option.speed;
  std::this_thread::sleep_for(duration);
  Tui.reflush();
}

void Controller::setData(DataGenerator::Unique_ptr source) {
  if (source->size == 0) return;
  data_generator = std::move(source);
  if (isIdle()) initData();
}

void Controller::toggle() {
  use enum Phase;
  switch (phase) {
  case Ready: start_sort().detach(); break;
  case Running: pause(); break;
  case Paused: resume(); break;
  case Done: initData(); break;
  default: break;
  }
}

void Controller::pause() {
  use enum Phase;
  if (phase == Running) phase = Paused;
  cond_stop.notify_all();
};

void Controller::resume() {
  use enum Phase;
  if (phase == Paused) phase = Running;
  cond_stop.notify_all();
}

use enum Option::Method;
Option::Map Option::map{
  {Bubble, "冒泡排序"},
  {Insert, "插入排序"},
  {Select, "选择排序"},
  {Quick, "快速排序"},
  {Merge, "归并排序"},
  {Heap, "堆排序"},
  {Shell, "希儿排序"},
  {Bucket, "桶排序"},
  {Radix, "基数排序"},
};

fn Controller::start_sort() -> std::thread {
  use enum Option::Method;
  use enum Phase;
  if (phase != Ready) throw std::exception{};

  start_time = time::steady_clock::now();
  resetRecord();
  return std::thread([&] {
    phase = Running;
    switch (option.method) {
    case Bubble: data.bubble_sort(); break;
    case Insert: data.insert_sort(); break;
    case Select: data.select_sort(); break;
    case Quick: data.quick_sort(); break;
    case Merge: data.merge_sort(); break;
    case Heap: data.heap_sort(); break;
    case Shell: data.shell_sort(); break;
    case Bucket: data.bucket_sort(); break;
    case Radix: data.radix_sort(); break;
    case Count: break;
    }

    done();
    end_time = time::steady_clock::now();
    // 排完之后画个绿三角，也就是为个好看
    forEach([&](int index, int _) {
      if (get_state_of(index) != Status::Sorted) {
        set_sorted(index);
        wait();
      }
    });
  });
}
} // namespace lingshin
