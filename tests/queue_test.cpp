#include "Queue.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

int main() {
  Queue<int, 3> queue;

  // New queue should be empty.
  assert(queue.isEmpty());
  assert(queue.size() == 0);

  // Test enqueue and FIFO order.
  queue.enqueue(10);
  queue.enqueue(20);
  queue.enqueue(30);

  assert(queue.isFull());
  assert(queue.size() == 3);
  assert(queue.front() == 10);

  // Test overflow.
  bool overflowCaught = false;

  try {
    queue.enqueue(40);
  } catch (const std::overflow_error &) {
    overflowCaught = true;
  }

  assert(overflowCaught);

  // Remove the first item, then insert another.
  // This makes the rear index wrap around.
  assert(queue.dequeue() == 10);
  queue.enqueue(40);

  // Remaining items must still follow FIFO order.
  assert(queue.dequeue() == 20);
  assert(queue.dequeue() == 30);
  assert(queue.dequeue() == 40);

  assert(queue.isEmpty());

  // Test underflow.
  bool underflowCaught = false;

  try {
    queue.dequeue();
  } catch (const std::underflow_error &) {
    underflowCaught = true;
  }

  assert(underflowCaught);

  // Test clear.
  queue.enqueue(50);
  queue.enqueue(60);
  queue.clear();

  assert(queue.isEmpty());
  assert(queue.size() == 0);

  std::cout << "All Queue tests passed!\n";

  return 0;
}
