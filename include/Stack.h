#ifndef STACK_H
#define STACK_H

#include <stdexcept>

template <typename T, int Capacity = 52> class Stack {
private:
  T items[Capacity];
  int topIndex = -1;

public:
  void push(const T &item) {
    if (topIndex == Capacity - 1) {
      throw std::overflow_error("Stack is full.");
    }

    items[++topIndex] = item;
  }

  T pop() {
    if (isEmpty()) {
      throw std::underflow_error("Stack is empty.");
    }

    return items[topIndex--];
  }

  T &top() {
    if (isEmpty()) {
      throw std::underflow_error("Stack is empty.");
    }

    return items[topIndex];
  }

  const T &top() const {
    if (isEmpty()) {
      throw std::underflow_error("Stack is empty.");
    }

    return items[topIndex];
  }

  T &at(int index) {
    if (index < 0 || index >= size()) {
      throw std::out_of_range("Invalid stack index.");
    }

    return items[index];
  }

  const T &at(int index) const {
    if (index < 0 || index >= size()) {
      throw std::out_of_range("Invalid stack index.");
    }

    return items[index];
  }

  bool isEmpty() const { return topIndex == -1; }

  bool isFull() const { return topIndex == Capacity - 1; }

  int size() const { return topIndex + 1; }

  void clear() { topIndex = -1; }
};

#endif
