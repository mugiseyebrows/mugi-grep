cmake_minimum_required(VERSION 3.16)
project(tst_mugigrep VERSION 1.0 LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Test Widgets)
qt_add_executable(tst_mugigrep
    tst_mugigrep.cpp tst_mugigrep.h
    ../src/mugigrep.cpp ../src/mugigrep.h
    ../src/regexppair.cpp ../src/regexppair.h
    ../src/searchhit.cpp ../src/searchhit.h
)
target_include_directories(tst_mugigrep PRIVATE ../src)

target_link_libraries(tst_mugigrep PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Test
    Qt::Widgets
)

