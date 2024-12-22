#include <App.h>
#include <Int.h>
#include <Tools.h>
#include <algorithm>
#include <chrono>
#include <mutex>
#include <thread>

namespace lingshin {
Controller &App = Controller::getInstance();

Controller Controller::app;

using namespace std::chrono_literals;

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
  for (int index = 0; index < data.size(); ++index) {
    callback(index, data.at(index));
  }
}

fn Controller::timePast() -> time::seconds {
  if (phase == Phase::Ready) return {0s};
  let now = phase == Phase::Done ? end_time : time::steady_clock::now();
  return tool::cast<time::seconds>(now - start_time);
}

void Controller::wait() {
  use enum Phase;
  var lock = std::unique_lock<std::mutex>{statusLock};
  if (phase == Paused) cond_stop.wait(lock, [&] { return phase != Paused; });
  if (option.speed < 0.01) {
    phase = Paused;
  } else {
    let duration = 100ms / option.speed;
    std::this_thread::sleep_for(duration);
  }
  Tui.reflush();
}

void Controller::setData(DataGenerator::Unique_ptr source) {
  if (source->size == 0) return;
  data_generator = std::move(source);
  if (isIdle()) initData();
}

void Controller::toggle() {
  use enum Phase;
  if (phase == Ready)
    start_sort().detach();
  else if (phase == Paused)
    resume();
  else if (phase == Running)
    pause();
  else if (phase == Done)
    initData();
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

fn Controller::start_sort() -> std::thread {
  use enum Option::Method;
  use enum Phase;
  if (phase != Ready) return {};
  phase = Running;
  start_time = time::steady_clock::now();
  return std::thread([&] {
    switch (option.method) {
    case Bubble: data.bubble_sort(); break;
    case Insert: data.insert_sort(); break;
    case Select: data.select_sort(); break;
    case Quick: data.quick_sort(); break;
    case Merge: data.merge_sort(); break;
    }

    done();

    forEach([&](int index, int) {
      if (get_state_of(index) != Status::Sorted) {
        set_sorted(index);
        wait();
      }
    });

    end_time = time::steady_clock::now();
    Tui.reflush();
  });
}
} // namespace lingshin
