#include "Queue.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

int main() {
  Queue<int, 3> queue;

  assert(queue.isEmpty());
  assert(queue.size() == 0);

  // Enqueue and FIFO order.
  queue.enqueue(10);
  queue.enqueue(20);
  queue.enqueue(30);

  assert(queue.isFull());
  assert(queue.size() == 3);
  assert(queue.front() == 10);

  // Overflow.
  bool overflowCaught = false;

  try {
    queue.enqueue(40);
  } catch (const std::overflow_error &) {
    overflowCaught = true;
  }

  assert(overflowCaught);

  // Circular wraparound.
  assert(queue.dequeue() == 10);
  queue.enqueue(40);

  assert(queue.dequeue() == 20);
  assert(queue.dequeue() == 30);
  assert(queue.dequeue() == 40);

  assert(queue.isEmpty());

  // Underflow.
  bool underflowCaught = false;

  try {
    queue.dequeue();
  } catch (const std::underflow_error &) {
    underflowCaught = true;
  }

  assert(underflowCaught);

  // Clear.
  queue.enqueue(50);
  queue.enqueue(60);
  queue.clear();

  assert(queue.isEmpty());
  assert(queue.size() == 0);

  // Indexed access.
  Queue<int, 5> indexedQueue;

  indexedQueue.enqueue(10);
  indexedQueue.enqueue(20);
  indexedQueue.enqueue(30);

  assert(indexedQueue.at(0) == 10);
  assert(indexedQueue.at(1) == 20);
  assert(indexedQueue.at(2) == 30);

  // Indexed access after circular wraparound.
  indexedQueue.dequeue();
  indexedQueue.dequeue();

  indexedQueue.enqueue(40);
  indexedQueue.enqueue(50);

  assert(indexedQueue.at(0) == 30);
  assert(indexedQueue.at(1) == 40);
  assert(indexedQueue.at(2) == 50);

  // Invalid index.
  bool invalidIndexCaught = false;

  try {
    indexedQueue.at(3);
  } catch (const std::out_of_range &) {
    invalidIndexCaught = true;
  }

  assert(invalidIndexCaught);

  std::cout << "All Queue tests passed!\n";

  return 0;
}
