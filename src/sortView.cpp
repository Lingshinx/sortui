#include <App.h>
#include <UI.h>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <string>
namespace lingshin {
using namespace ftxui;

fn UI::viewPage() -> ftxui::Component {
  return Renderer([] { return gauge(0.5); });
}

fn makeBars() {
  use enum Controller::Status;
  var length = App.getDataView().size();
  var bars = Elements(length);
  var max = App.getMax();
  App.forEach([&](int index, int value) {
    var status = App.get_state_of(index);
    var length = static_cast<float>(value) / max;
    var bar = gauge(length);
    if (status == Swapping)
      bar |= color(Color::Yellow);
    else if (status == Comparing)
      bar |= color(Color::Green);
    bars.push_back(bar);
  });
  return bars;
}

fn verticalView() { return vbox(makeBars()) | border; }
fn HorizontalView() { return hbox(makeBars()) | border; }

fn info(const std::string &label, int value) -> Element {
  return hbox({text(label), filler(), text(std::to_string(value))}) | flex;
};

fn infoPanel() {
  var record = App.getRecord();
  var swapTimes = record.swpTimes;
  var compareTimes = record.swpTimes;
  return window(text("info"), vbox({info("交换次数", swapTimes),
                                    info("比较次数", compareTimes)}));
}

fn controller() {}

} // namespace lingshin
