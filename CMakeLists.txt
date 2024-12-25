# 这些是Ai 生成的, CMake 得花时间研究
# 但我的所有代码都是纯键盘打的, 这是作为vimer 的尊严()
cmake_minimum_required(VERSION 3.10)
project(Sortui)

set(CMAKE_CXX_STANDARD 20)

# Add the FTXUI library
include(FetchContent)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
  GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

# Add include directory
include_directories(include)

# Add all source files in the src directory
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(sortui ${SOURCES})

# Link the FTXUI library
target_link_libraries(sortui PRIVATE ftxui::screen ftxui::dom ftxui::component)

# Add include directory for target
target_include_directories(sortui PRIVATE ${CMAKE_SOURCE_DIR}/include)
