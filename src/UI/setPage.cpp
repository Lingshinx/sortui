#include "App.h"
#include <Alias.h>
#include <UI/SetPage.h>
#include <UI/UI.h>
namespace lingshin {
use namespace ftxui;
use namespace std::string_literals; // 好用

// ---静态定义区------
String SetPage::Ctrl_w = "\x17"; // 本来应该用<Ctrl+S>的，不过好像有些问题
String SetPage::message;
fn SetPage::input_option() {
  return InputOption{
    .transform =
      [](InputState state) {
        if (state.focused) {
          state.element |= underlined;
        } else if (state.hovered) {
          state.element |= bgcolor(Color::Palette1::Default);
        }
        return state.element;
      },
    .multiline = false,
  };
}
fn SetPage::numberInput(const String &label, String &strRef) {
  let input = Input(&strRef, input_option()) | CatchEvent([](Event e) {
    return e.is_character() && !isdigit(e.character()[0]);
  });
  return Renderer(input, [=] { return hbox({text(label), input->Render()}); });
}
// -------------------------

// ---随机数据面板---------------------
SetPage::Random::Random() {
  let minInput = numberInput("最小值: ", min);
  let maxInput = numberInput("最大值: ", max);
  let sizeInput = numberInput("长度: ", size);
  let apply = [&] {
    try {
      let maximum = stoi(max);
      let minimum = stoi(min);
      let length = stoi(size);
      let range = DataGenerator::Range{maximum, minimum};
      App.setData(DataGenerator::from(range, length));
      Tui.tabIndex = UI::View;
      message.clear();
    } catch (std::invalid_argument e) {
      message = "error: 有一项是空的";
    } catch (std::out_of_range e) {
      message = "error: 太长了";
    };
  };
  let button = Button("Apply", apply);
  let container = Container::Vertical({
    minInput,
    maxInput,
    sizeInput,
    button,
  });
  self = Renderer(container, [=] {
    return vbox({
      minInput->Render(),
      maxInput->Render(),
      sizeInput->Render(),
      separatorEmpty(),
      button->Render() | align_right,
    });
  }) | CatchEvent([=](Event e) {
    if (e == Event::Special(Ctrl_w)) {
      apply();
      return true;
    }
    return false;
  });
} // ---

// ---自定义数据面板--------------
fn SetPage::Array::to_string(const Vec &data) -> String {
  var str = String{};
  if (data.empty()) return "还没有数据"s;
  for (let &it : data)
    str += std::to_string(it) + ' ';
  return str;
};

SetPage::Array::Array() {
  let onenter = [&](Event e) {
    if (e == Event::Character("\n")) {
      if (it.empty()) return true;
      try {
        data.push_back(std::stoi(it));
        message.clear();
      } catch (std::invalid_argument e) {
        message = "error: 空的"s;
      } catch (std::out_of_range e) {
        message = "error: 太长了";
      }
      it.clear();
      return true;
    }
    return false;
  };
  let filter = [](Event e) {
    return e.is_character() && !isdigit(e.character()[0]);
  };
  let input =
    Input(&it, "0", input_option()) | CatchEvent(filter) | CatchEvent(onenter);
  let apply = [&] {
    App.setData(DataGenerator::from(data));
    Tui.tabIndex = UI::View;
  };
  let button = Button("Apply", apply);
  let container = Container::Vertical({input, button});
  self = Renderer(container, [&, button, input] {
    return vbox({
      paragraph(to_string(data)) |
        ftxui::size(WidthOrHeight::HEIGHT, Constraint::GREATER_THAN, 2),
      hbox({
        text("请输入"),
        input->Render(),
      }),
      separatorEmpty(),
      button->Render() | align_right,
    });
  }) | CatchEvent([=](Event e) {
    if (e == Event::Special(Ctrl_w)) {
      apply();
      return true;
    }
    return false;
  });
}

SetPage::SetPage() {
  let tab = Container::Tab(
    {
      random.self,
      array.self,
    },
    &which);
  let container = Container::Vertical({
    tabToggle,
    tab,
  });

  self = Renderer(container, [&, tab] {
    use enum WidthOrHeight;
    use enum Constraint;
    return vbox(vbox({
                  tabToggle->Render() | center,
                  separatorLight() | color(Color::Blue),
                  tab->Render() | borderEmpty,
                }) |
                  borderStyled(Color::Blue) | ftxui::size(WIDTH, EQUAL, 40),
             text(message) | color(Color::Red) | center) |
           center;
  }) | CatchEvent([&](Event e) {
    if (e == Event::Character('q')) {
      Tui.tabIndex = UI::Home;
      return true;
    }
    if (e == Event::Tab) {
      which = !which;
      return true;
    }
    return false;
  });
}
} // namespace lingshin
