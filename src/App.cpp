#include "Int.h"
#include <App.h>
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
  let now = time::steady_clock::now();
  return std::chrono::duration_cast<time::seconds>(now - start_time);
}

void Controller::wait() {
  use enum Phase;
  var lock = std::unique_lock<std::mutex>{statusLock};
  if (phase == Paused) cond_stop.wait(lock, [&] { return phase != Paused; });
  let duration = 100ms / option.speed;
  Tui.reflush();
  std::this_thread::sleep_for(duration);
}

void Controller::setData(DataGenerator &source) {
  var index = 0;
  if (source.size == 0) return;
  phase = Controller::Phase::Ready;
  data.resize(source.size);
  isSorted.resize(source.size);
  for (var &it : data) {
    it = source[index++];
  }
  max = std::ranges::max(data);
}

void Controller::toggle() {
  use enum Phase;
  if (phase == Ready) start_sort().detach();
  if (phase == Paused) resume();
  if (phase == Running) pause();
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

    phase = Done;
    forEach([&](int index, int) {
      if (get_state_of(index) != Status::Sorted) {
        set_sorted(index);
        wait();
      }
    });
    Tui.reflush();
  });
}
} // namespace lingshin
