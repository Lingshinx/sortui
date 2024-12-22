#pragma once
#include <Alias.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
namespace lingshin {
class SetPage {
public:
  ftxui::Component self;
  SetPage();

private:
  static fn numberInput(const String &label, String &strRef);
  // static ftxui::InputOption input_option;
  static String message;
  int which = 0;
  static String Ctrl_w;
  std::vector<String> entries{"随机数据", "手动数据"};
  ftxui::Component tabToggle =
    ftxui::Toggle(&entries, &which) | color(ftxui::Color::Blue);
  static fn input_option();

  struct Array {
    use Vec = std::vector<int>;
    Vec data;
    String it;
    fn to_string(const Vec &data) -> String;
    ftxui::Component self;
    Array();
  } array;

  struct Random {
    String min{"0"};
    String max{"99"};
    String size{"30"};
    ftxui::Component self;
    Random();
  } random;
};
} // namespace lingshin
