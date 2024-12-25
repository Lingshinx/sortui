#pragma once
#include <Alias.h>
#include <Option.h>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/node.hpp>

namespace lingshin {
class SortView {
public:
  SortView();
  ftxui::Component self;

private:
  static fn getMethod() -> String;
  static fn make_key(String meaning, String key) -> ftxui::Element;

  static fn viewPanel() -> ftxui::Element;

  class InfoPanel : public ftxui::Element {

    static fn status() -> ftxui::Element;
    static fn info(
      const String &label,
      String value,
      ftxui::Color colour = ftxui::Color{}
    ) -> ftxui::Element;

    static fn info(
      const String &label,
      int value,
      ftxui::Color colour = ftxui::Color{}
    ) -> ftxui::Element;

    static fn constructor(ftxui::Component)->ftxui::Element;

  public:
    InfoPanel(ftxui::Component);
    struct ControllerPanel : public ftxui::Component {
      ControllerPanel();
      static fn switchMethod();

    private:
      static fn speed();
      static fn setData();
      static fn methodSwitcher() -> ftxui::Component;
      static fn constructor() -> ftxui::Component;
      static fn formatSpeed(float speed) -> String;
      static fn nextSpeed(float speed) {
        return speed < 1 ? 2 : speed < 5 ? 5 : 0.5;
      }
      static fn speedIcon(float speed) -> String {
        return speed < 1 ? "󰾆 slow" : 
               speed < 5 ? "󰾅 norm" : "󰓅 fast";
      }
      static fn decorator(bool focused) {
        use namespace ftxui;
        return focused ? color(Color::Green) | bold | border 
                       : borderEmpty;
      }
    };

  };

};
}; // namespace lingshin
