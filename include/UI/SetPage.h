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
  static String message;
  static String Ctrl_w;
  static fn input_option();
  int which = 0;
  std::vector<String> entries{"随机数据", "手动数据"};
  ftxui::Component tabToggle
  = ftxui::Toggle(&entries, &which)
    | color(ftxui::Color::Blue);

  struct Array {
    ftxui::Component self;
    Array();
    
  private:
    use Vec = std::vector<int>;
    Vec data;
    String it;
    fn to_string(const Vec &data) -> String;
  } array;

  struct Random {
    ftxui::Component self;
    Random();

  private:
    String min{"0"};
    String max{"99"};
    String size{"30"};
  } random;
};
} // namespace lingshin
