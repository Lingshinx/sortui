#include "Alias.h"
#include <App.h>
#include <Option.h>
#include <Tools.h>
#include <UI/SortView.h>
#include <UI/UI.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/string.hpp>
#include <iomanip>
#include <sstream>
#include <string>
namespace lingshin {
use namespace ftxui;

fn SortView::make_key(std::string meaning, std::string key) -> Element {
  return hbox({
    text(' ' + meaning + ' ') | bold | bgcolor(Color::RGB(47, 51, 77)),
    text(key + ' ') | color(Color::Cyan) | bgcolor(Color::RGB(47, 51, 77)),
    text(" "),
  });
}

fn SortView::viewPanel() -> Element {
  use enum Controller::Status;
  var nums = Elements{};
  var bars = Elements{};
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
    let length = rate + (rate > 0.75 ? -0.01 : rate < 0.25 ? 0.01 : 0);
    let bar = gauge(length) | color(textColor);
    let num = text(std::to_string(value)) | bold | color(textColor);
    nums.push_back(num);
    bars.push_back(bar);
  });
  return hbox({vbox(nums), separatorLight(), vbox(bars) | flex}) |
         vscroll_indicator | yframe | flex | border;
}

fn SortView::InfoPanel::info(
  const String &label, String value, Color colour) -> Element {
  return hbox({text(label), filler(), text(value) | bold | color(colour)}) |
         flex;
}

fn SortView::InfoPanel::info(
  const String &label, int value, Color colour) -> Element {
  return info(label, std::to_string(value), colour);
}

inline fn SortView::getMethod() -> String {
  return Option::map.at(App.option.method);
}

fn SortView::InfoPanel::constructor(Component controller) -> ftxui::Element {
  let &record = App.getRecord();
  let swapTimes = record.swpTimes;
  let compareTimes = record.cmpTimes;
  let spaceUsed = record.spaceUsed;
  let size = App.getDataView().size();
  let inside =
    vbox({
      text(getMethod()) | underlined | bold | color(Color::Pink1),
      separatorEmpty(),
      info(" 交换次数:", swapTimes, Color::Blue),
      info(" 比较次数:", compareTimes, Color::Yellow),
      info(" 空间使用:", spaceUsed, Color::Green),
      info("󰉻 数据长度:", size, Color::Magenta),
      separatorEmpty(),
      info(" 耗时:", tool::formatDuration(App.timePast()), Color::Red),
    }) |
    borderEmpty;
  return window(status() | center,
           vbox({
             inside,
             separatorLight(),
             controller->Render(),
           })) |
         ftxui::size(WidthOrHeight::WIDTH, Constraint::GREATER_THAN, 20);
}

SortView::InfoPanel::InfoPanel(Component controller)
  : Element(constructor(controller)) {}

fn SortView::InfoPanel::ControllerPanel::formatSpeed(float speed) -> String {
  use namespace std;
  return speedIcon(speed) + " " +
         (stringstream() << fixed << setprecision(1) << speed).str() + "x";
}

fn SortView::InfoPanel::ControllerPanel::speed() {
  var &speed = App.option.speed;
  return Button({.on_click = [&] { speed = nextSpeed(speed); },
    .transform =
      [&](const EntryState &entry) {
        return text(formatSpeed(speed)) | center | decorator(entry.focused);
      }});
}

fn SortView::InfoPanel::ControllerPanel::switchMethod() {
  use enum Option::Method;
  var &method = App.option.method;
  let methodNum = static_cast<int>(Count);
  let currentMethod = static_cast<int>(method);
  let nextMethod = (currentMethod + 1) % methodNum;
  method = static_cast<Option::Method>(nextMethod);
}

fn SortView::InfoPanel::ControllerPanel::methodSwitcher() -> Component {
  return Button(
    "切换方法", switchMethod, {.transform = [](const EntryState &entry) {
      return text(entry.label) | center | decorator(entry.focused);
    }});
}

fn SortView::InfoPanel::ControllerPanel::setData() {
  return Button("设置数据",
    [] { Tui.tabIndex = UI::Set; },
    {.transform = [](const EntryState &entry) {
      return text(entry.label) | center | decorator(entry.focused);
    }});
}

fn SortView::InfoPanel::ControllerPanel::constructor() -> Component {
  return Container::Vertical({
    speed(),
    methodSwitcher(),
    setData(),
  });
}

SortView::InfoPanel::ControllerPanel::ControllerPanel()
  : Component(constructor()){};

fn SortView::InfoPanel::status() -> Element {
  use enum Controller::Phase;
  let &status = App.phase;
  if (status == Running) return text("Running") | bold | color(Color::Yellow);
  if (status == Ready) return text("Ready") | bold | color(Color::Blue);
  if (status == Paused) return text("Paused") | bold | color(Color::Red);
  return text("Done") | color(Color::Green);
}

SortView::SortView() {
  var controlPanel = InfoPanel::ControllerPanel();

  let hotKeyHint = hflow({
                     make_key("开始/暂停", "Space"),
                     make_key("加速", ">"),
                     make_key("减速", "<"),
                     make_key("Quit", "q"),
                     make_key("切换方法", "Tab"),
                   }) |
                   border |
                   size(WidthOrHeight::HEIGHT, Constraint::GREATER_THAN, 3);

  self = Renderer(controlPanel, [=] {
    return vbox({
      hbox({
        viewPanel(),
        InfoPanel(controlPanel),
      }),
      hotKeyHint,
    });
  }) | CatchEvent([&](Event e) {
    if (e == Event::Character(' ')) {
      App.toggle();
      return true;
    }
    if (e == Event::Character('q')) {
      Tui.tabIndex = UI::Home;
      return true;
    }
    var &speed = App.option.speed;
    let step = speed < 1    ? 0.1
               : speed < 2  ? 0.2
               : speed < 5  ? 0.5
               : speed < 10 ? 1
                            : 10;

    if (e == Event::Character('>')) {
      speed += step;
      return true;
    }
    if (e == Event::Character('<')) {
      speed = speed - step > 0.01 ? speed - step : 0;
      return true;
    }
    if (e == Event::Tab) {
      InfoPanel::ControllerPanel::switchMethod();
      return true;
    }
    return false;
  });
}

} // namespace lingshin
