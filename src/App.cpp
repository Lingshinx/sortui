#include "Int.h"
#include <App.h>
#include <algorithm>
#include <iostream>
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
  var pair = getRecord();
  if (let now = pair.nowCmping; now && in(*now, index))
    return Status::Comparing;
  if (let now = pair.nowSwaping; now && in(*now, index))
    return Status::Swapping;
  return Status::NotActive;
};

void Controller::forEach(CallBack callback) {
  for (int index = 0; index < data.size(); ++index) {
    callback(index, data.at(index));
  }
}

void Controller::wait() {
  use enum Phase;
  var lock = std::unique_lock<std::mutex>{statusLock};
  if (phase == Paused) cond_stop.wait(lock, [&] { return phase != Paused; });
  let duration = 100ms / option.speed;
  std::this_thread::sleep_for(duration);
}

void Controller::setData(DataGenerator &source) {
  var index = 0;
  data.resize(source.size);
  for (var &it : data) {
    it = source[index++];
  }
  max = std::ranges::max(data);
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
  if (phase != StandBy) return {};
  phase = Running;
  return std::thread([&] {
    switch (option.method) {
    case Bubble:
      data.bubble_sort();
      break;
    case Insert:
      data.insert_sort();
      break;
    case Select:
      data.select_sort();
      break;
    case Quick:
      data.quick_sort();
      break;
    case Merge:
      data.merge_sort();
      break;
    }
    phase = Done;
  });
}
} // namespace lingshin
