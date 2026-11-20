#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>

template <typename T, int Capacity = 52>
class Queue {
private:
    T items[Capacity];
    int frontIndex = 0;
    int rearIndex = -1;
    int count = 0;

public:
    void enqueue(const T& item) {
        if (isFull()) {
            throw std::overflow_error("Queue is full.");
        }

        rearIndex = (rearIndex + 1) % Capacity;
        items[rearIndex] = item;
        count++;
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty.");
        }

        T item = items[frontIndex];
        frontIndex = (frontIndex + 1) % Capacity;
        count--;

        return item;
    }

    T& front() {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty.");
        }

        return items[frontIndex];
    }

    T& at(int index) {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Invalid queue index.");
        }

        int actualIndex = (frontIndex + index) % Capacity;
        return items[actualIndex];
    }

    const T& at(int index) const {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Invalid queue index.");
        }

        int actualIndex = (frontIndex + index) % Capacity;
        return items[actualIndex];
    }

    bool isEmpty() const {
        return count == 0;
    }

    bool isFull() const {
        return count == Capacity;
    }

    int size() const {
        return count;
    }

    void clear() {
        frontIndex = 0;
        rearIndex = -1;
        count = 0;
    }
};

#endif

