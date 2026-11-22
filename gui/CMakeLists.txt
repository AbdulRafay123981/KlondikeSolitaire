cmake_minimum_required(VERSION 3.21)

project(KlondikeGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.12 REQUIRED COMPONENTS Widgets)

add_executable(KlondikeGUI
    main.cpp
    MainWindow.h
    MainWindow.cpp
    ../src/Deck.cpp
    ../src/GameBoard.cpp
)

target_include_directories(KlondikeGUI PRIVATE
    ../include
)

target_link_libraries(KlondikeGUI PRIVATE
    Qt6::Widgets
)
