#include "Stack.h"

#include <iostream>
#include <stdexcept>

int main() {
  Stack<int, 3> stack;

  if (!stack.isEmpty()) {
    std::cout << "FAIL: New stack should be empty.\n";
    return 1;
  }

  stack.push(10);
  stack.push(20);
  stack.push(30);

  if (stack.top() != 30 || stack.size() != 3) {
    std::cout << "FAIL: Push or top failed.\n";
    return 1;
  }

  if (!stack.isFull()) {
    std::cout << "FAIL: Stack should be full.\n";
    return 1;
  }

  try {
    stack.push(40);
    std::cout << "FAIL: Full stack accepted another item.\n";
    return 1;
  } catch (const std::overflow_error &) {
    std::cout << "PASS: Stack overflow detected.\n";
  }

  if (stack.pop() != 30 || stack.pop() != 20 || stack.pop() != 10) {
    std::cout << "FAIL: Stack does not follow LIFO.\n";
    return 1;
  }

  if (!stack.isEmpty()) {
    std::cout << "FAIL: Stack should be empty after popping.\n";
    return 1;
  }

  try {
    stack.pop();
    std::cout << "FAIL: Empty stack allowed pop.\n";
    return 1;
  } catch (const std::underflow_error &) {
    std::cout << "PASS: Stack underflow detected.\n";
  }

  stack.push(50);
  stack.clear();

  if (!stack.isEmpty()) {
    std::cout << "FAIL: Clear did not empty the stack.\n";
    return 1;
  }

  std::cout << "PASS: Push, pop and top work correctly.\n";
  std::cout << "PASS: Stack size and clear work correctly.\n";
  std::cout << "All stack tests passed!\n";

  return 0;
}
