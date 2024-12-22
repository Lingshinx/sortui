#include <UI.h>

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
    &tabIndex
  );
}

} // namespace lingshin
