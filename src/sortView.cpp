#include <App.h>
#include <Tools.h>
#include <UI.h>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/string.hpp>
#include <string>
namespace lingshin {
using namespace ftxui;

fn viewPanel() {
  use enum Controller::Status;
  var bars = Elements{};
  var nums = Elements{};
  let length = App.getDataView().size();
  bars.reserve(length);
  let max = App.getMax();
  App.forEach([&](int index, int value) {
    let status = App.get_state_of(index);
    let textColor = (status == Swapping)    ? Color::Blue
                    : (status == Comparing) ? Color::Yellow
                    : (status == Sorted)    ? Color::Green
                                            : Color{};
    let rate = static_cast<float>(value) / max;
    let length = rate + (rate > 0.5 ? -0.01 : 0.01);
    let bar = gauge(length) | color(textColor);
    let num = text(std::to_string(value)) | bold | color(textColor);
    nums.push_back(num);
    bars.push_back(bar);
  });
  return hbox({vbox(nums), separatorLight(), vbox(bars) | flex}) | flex |
         border;
}

fn info(const std::string &label, std::string value) -> Element {
  return hbox({text(label), filler(), text(value)}) | flex;
}

fn info(const std::string &label, int value) -> Element {
  return info(label, std::to_string(value));
}

fn infoPanel() {
  let &record = App.getRecord();
  let swapTimes = record.swpTimes;
  let compareTimes = record.cmpTimes;
  return vbox({info("交换次数", swapTimes),
    info("比较次数", compareTimes),
    info("耗时", tool::formatDuration(App.timePast()))});
}

fn controller() { return text("hello"); }

fn status() {
  use enum Controller::Phase;
  let &status = App.phase;
  if (status == Running) return text("Running") | bold | color(Color::Yellow);
  if (status == Ready) return text("Ready") | bold | color(Color::Blue);
  if (status == Paused) return text("Paused") | bold | color(Color::Red);
  return text("Done") | color(Color::Green);
}

fn UI::viewPage() -> ftxui::Component {
  return Renderer([] {
    return hbox({
      viewPanel(),
      window(status() | center,
        vbox({
          infoPanel(),
          separatorLight(),
          controller(),
        })) |
        size(WidthOrHeight::WIDTH, Constraint::GREATER_THAN, 30),
    });
  }) | CatchEvent([&](Event e) {
    if (e == Event::Character(' ')) {
      App.toggle();
      return true;
    }
    if (e == Event::Character('q')) {
      tabIndex = Home;
      return true;
    }
    return false;
  });
} // namespace lingshin
} // namespace lingshin
