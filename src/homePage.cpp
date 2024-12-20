#include "DataGenerator.h"
#include <App.h>
#include <UI.h>
#include <ftxui/component/component.hpp>
namespace lingshin {
use namespace ftxui;

fn logo() -> Element {
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

static let mySign = hbox({
                      text("Sortui 可视化排序") | color(Color::Blue),
                      text(" ———— "),
                      text(" lingshin") | color(Color::Pink1),
                    }) |
                    center;

static let style = ButtonOption{.transform = [](const EntryState &s) {
  let decorator = s.focused ? bold | color(Color::Cyan) : nothing;
  return text(s.label) | decorator;
}};

fn HotButton(
  const std::string &label, Closure onclick, const std::string &hotkey) {
  let button = Button(label, onclick, style);
  return Renderer(button, [=] {
    return hbox({
      button->Render(),
      filler(),
      text(hotkey) | color(Color::Yellow) | align_right,
    });
  }) | flex;
}

fn UI::homePage() -> Component {
  use enum Controller::Phase;
  let onstart = [&] {
    if (App.phase == StandBy) App.setData(DataGenerator::from(0, 99));
    tabIndex = View;
  };
  let onconfig = [&] { tabIndex = Set; };
  let onquit = [&] { screen.Exit(); };

  let start = HotButton("  Start", onstart, "S");
  let config = HotButton("  Config", onconfig, "C");
  let quit = HotButton("󰈆  Quit", onquit, "Q");

  let layout = Container::Vertical({
    start,
    config,
    quit,
  });

  return Renderer(layout, [=] {
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
