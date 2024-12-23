#pragma once
#include <Alias.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/task.hpp>

namespace lingshin {
class HomePage {
  static const ftxui::Element mySign;

  fn logo() -> ftxui::Element;
  fn buttonStyle();
  fn HotButton(const String &label,
    ftxui::Closure onclick,
    const String &hotkey) -> ftxui::Component;

public:
  ftxui::Component self;
  HomePage();
};
} // namespace lingshin
