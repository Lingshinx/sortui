#pragma once
#include <Alias.h>
#include <cstdio>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/task.hpp>
namespace lingshin {
extern class UI {
public:
  static fn getInstance() -> UI & { return _statik; }
  fn loop() { screen.Loop(tabs()); };
  fn reflush() { screen.Post(ftxui::Event::Custom); }

  static const int Home = 0;
  static const int View = 1;
  static const int Set = 2;
  int tabIndex = Home;

private:
  static UI _statik;
  ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();

  fn tabs() -> ftxui::Component;
  fn homePage() -> ftxui::Component;
  fn viewPage() -> ftxui::Component;
  fn setPage() -> ftxui::Component;

  UI(){};
} & Tui;

} // namespace lingshin
