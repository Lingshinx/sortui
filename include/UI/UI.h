#pragma once
#include "HomePage.h"
#include "SetPage.h"
#include "SortView.h"
#include <Alias.h>
#include <ftxui/component/screen_interactive.hpp>

// UI 我就不注释了, 得专门去学ftxui 项目的文档才看得懂的
namespace lingshin {
extern class UI {
public:
  static fn getInstance() -> UI & { return _statik; }
  fn loop() { screen.Loop(tabs()); };
  fn reflush() { screen.Post(ftxui::Event::Custom); }
  fn close() { screen.Exit(); };

  static const int Home = 0;
  static const int View = 1;
  static const int Set = 2;
  int tabIndex = Home;

private:
  static UI _statik;
  ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();

  fn tabs() -> ftxui::Component;
  SortView viewpage;
  HomePage homepage;
  SetPage setpage;
  UI(){};
} & Tui;

} // namespace lingshin
