#include "LinkedList.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

int main() {
  LinkedList<int> list;

  // New list should be empty.
  assert(list.isEmpty());
  assert(list.size() == 0);

  // Test insertion at the front.
  list.pushFront(10);
  list.pushFront(20);
  list.pushFront(30);

  assert(list.size() == 3);
  assert(list.front() == 30);

  // Test accessing elements by index.
  assert(list.getAt(0) == 30);
  assert(list.getAt(1) == 20);
  assert(list.getAt(2) == 10);

  // Test removal from the front.
  assert(list.popFront() == 30);
  assert(list.popFront() == 20);

  assert(list.size() == 1);
  assert(list.front() == 10);

  // Test invalid index.
  bool indexErrorCaught = false;

  try {
    list.getAt(5);
  } catch (const std::out_of_range &) {
    indexErrorCaught = true;
  }

  assert(indexErrorCaught);

  // Remove the final element.
  assert(list.popFront() == 10);
  assert(list.isEmpty());

  // Test removing from an empty list.
  bool underflowCaught = false;

  try {
    list.popFront();
  } catch (const std::underflow_error &) {
    underflowCaught = true;
  }

  assert(underflowCaught);

  // Test clearing a list.
  list.pushFront(40);
  list.pushFront(50);
  list.pushFront(60);

  list.clear();

  assert(list.isEmpty());
  assert(list.size() == 0);

  std::cout << "All Linked List tests passed!\n";

  return 0;
}
