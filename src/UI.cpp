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
      homepage.self,
      viewpage.self,
      setpage.self,
    },
    &tabIndex);
}

} // namespace lingshin
