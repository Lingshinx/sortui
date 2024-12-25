#include <App.h>
#include <DataGenerator.h>
#include <UI/HomePage.h>
#include <UI/UI.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/node.hpp>
namespace lingshin {
use namespace ftxui;

fn HomePage::logo() -> Element {
  return vbox({
    text("                                            ▄▄  "),
    text(" ▄█████▄█                  ██╗              ██╗ "),
    text("▄██╔═══▀█                  ██║              ╚═╝ "),
    text("▀███▄     ▄██▀██▄▀███▄█████████▀███╗ ▀███╗▀███╗ "),
    text("  ▀█████▄██╔═══▀██ ██╔═▀▀══██║   ██║   ██║  ██║ "),
    text("▄     ▀████║    ██ ██║     ██║   ██║   ██║  ██║ "),
    text("██     ████║   ▄██ ██║     ██║   ██║   ██║  ██║ "),
    text("█▀██████══▀█████▀▄████╗    ▀████╗▀████▀███▄████╗"),
    text("  ╚════╝   ╚═══╝ ╚════╝     ╚═══╝ ╚══╝ ╚═══════╝"),
  });
}

const Element HomePage::mySign =
  hbox({
    text("Sortui 可视化排序") | color(Color::Blue),
    text(" ———— "),
    text(" lingshin") | color(Color::Pink1),
  }) |
  center;

fn HomePage::buttonStyle() {
  return ButtonOption{.transform = [](const EntryState &s) {
    let decorator = s.focused ? bold | color(Color::Cyan) : nothing;
    return text(s.label) | decorator;
  }};
}

fn HomePage::HotButton(
  const String &label, Closure onclick, const String &hotkey) -> Component {
  let button = Button(label, onclick, buttonStyle());
  return Renderer(button, [=] {
    return hbox({
      button->Render(),
      filler(),
      text(hotkey) | color(Color::Yellow) | align_right,
    });
  }) | flex;
}

HomePage::HomePage() {
  use enum Controller::Phase;
  let onstart = [&] {
    App.option.onfresh = [] { Tui.reflush(); };
    if (App.phase == StandBy) App.setData(DataGenerator::from(0, 99, 30));
    Tui.tabIndex = UI::View;
  };
  let onconfig = [&] { Tui.tabIndex = UI::Set; };
  let onquit = [&] { Tui.close(); };

  let start = HotButton("  Start", onstart, "S");
  let config = HotButton("  Config", onconfig, "C");
  let quit = HotButton("󰈆  Quit", onquit, "Q");

  let layout = Container::Vertical({
    start,
    config,
    quit,
  });

  self = Renderer(layout, [=, this] {
    return vbox({
             separatorEmpty(),
             logo() | color(Color::Blue),
             separatorEmpty(),
             start->Render(),
             separatorEmpty(),
             config->Render(),
             separatorEmpty(),
             quit->Render(),
             separatorEmpty(),
             mySign,
           }) |
           center;
  }) | CatchEvent([=](Event e) {
    if (e == Event::Character('s')) {
      onstart();
      return true;
    };
    if (e == Event::Character('c')) {
      onconfig();
      return true;
    };
    if (e == Event::Character('q')) {
      onquit();
      return true;
    };
    return false;
  });
}
} // namespace lingshin
