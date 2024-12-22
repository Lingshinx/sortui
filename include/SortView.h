#pragma once
#include <Alias.h>
#include <App.h>
#include <Option.h>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/node.hpp>
#include <map>
namespace lingshin {
class SortView {
public:
  SortView();
  ftxui::Component self;

private:
  use Map = std::map<Option::Method, String>;
  static Map map;
  static fn getMethod() -> String;

  static fn make_key(String meaning, String key) -> ftxui::Element;

  class InfoPanel : public ftxui::Element {
    static fn info(const String &label,
      String value,
      ftxui::Color colour = ftxui::Color{}) -> ftxui::Element;
    static fn info(const String &label,
      int value,
      ftxui::Color colour = ftxui::Color{}) -> ftxui::Element;
    static fn constructor(ftxui::Component)->ftxui::Element;
    static fn status() -> ftxui::Element;

  public:
    struct ControllerPanel : public ftxui::Component {
      static fn methodSwitcher() -> ftxui::Component;
      static fn constructor() -> ftxui::Component;
      static fn nextSpeed(float speed) {
        return speed < 1 ? 2 : speed < 5 ? 5 : 0.5;
      }
      static fn speedIcon(float speed) -> String {
        return speed < 1 ? "󰾆 slow" : speed < 5 ? "󰾅 norm" : "󰓅 fast";
      }
      static fn formatSpeed(float speed) -> String;
      static fn decorator(bool focused) {
        use namespace ftxui;
        return focused ? color(Color::Green) | bold | border : borderEmpty;
      }
      static fn speed();
      static fn setData();

      static fn switchMethod();
      ControllerPanel();
    };

    InfoPanel(ftxui::Component);
  };

  static fn viewPanel() -> ftxui::Element;
};
}; // namespace lingshin
