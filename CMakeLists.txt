cmake_minimum_required(VERSION 3.20)

project(KlondikeSolitaire LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(KlondikeSolitaire
    src/main.cpp
    src/Deck.cpp
)

target_include_directories(KlondikeSolitaire PRIVATE include)

enable_testing()

add_executable(DeckTest
    tests/deck_test.cpp
    src/Deck.cpp
)

target_include_directories(DeckTest PRIVATE include)

add_test(NAME DeckFunctionalityTest COMMAND DeckTest)


add_executable(StackTest
    tests/stack_test.cpp
)

target_include_directories(StackTest PRIVATE include)

add_test(NAME StackFunctionalityTest COMMAND StackTest)

add_executable(QueueTest
    tests/queue_test.cpp
)

target_include_directories(QueueTest PRIVATE include)

add_test(NAME QueueFunctionalityTest COMMAND QueueTest)

add_executable(LinkedListTest
    tests/linked_list_test.cpp
)

target_include_directories(LinkedListTest PRIVATE include)

add_test(NAME LinkedListFunctionalityTest COMMAND LinkedListTest)
