#include "App.h"
#include "DataGenerator.h"
#include "ftxui/dom/elements.hpp"
#include <UI.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/task.hpp>

namespace lingshin {
use namespace ftxui;
UI &Tui = UI::getInstance();
UI UI::_statik;

fn UI::tabs() -> Component {
  return Container::Tab(
    {
      homePage(),
      viewPage(),
    },
    &tabIndex);
}

} // namespace lingshin
