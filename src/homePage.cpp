#include <App.h>
#include <UI.h>
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

static var mySign = hbox({
                        text("Sortui 可视化排序") | color(Color::Blue),
                        text(" ———— "),
                        text(" lingshin") | color(Color::Pink1),
                    }) |
                    center;

static var style = ButtonOption{.transform = [](const EntryState &s) {
  var decorator = s.focused ? bold | color(Color::Cyan) : nothing;
  return text(s.label) | decorator;
}};

fn HotButton(const std::string &label, Closure onclick,
             const std::string &hotkey) {

  var button = Button(label, onclick, style);
  return Renderer(button,
                  [=] {
                    return hbox({
                        button->Render(),
                        filler(),
                        text(hotkey) | color(Color::Yellow) | align_right,
                    });
                  }) |
         flex;
}

fn UI::homePage() -> Component {
  use enum Controller::Phase;
  var onstart = [&] { tabIndex = View; };
  var onconfig = [&] { tabIndex = Set; };
  var onquit = [&] { screen.Exit(); };

  var start = HotButton("  Start", onstart, "S");
  var config = HotButton("  Config", onconfig, "C");
  var quit = HotButton("󰈆  Quit", onquit, "Q");

  var layout = Container::Vertical({
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
  });
}
} // namespace lingshin
