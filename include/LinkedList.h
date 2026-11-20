#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdexcept>
#include <utility>

template <typename T> class LinkedList {
private:
  struct Node {
    T data;
    Node *next;

    Node(const T &value) : data(value), next(nullptr) {}
  };

  Node *head = nullptr;
  int count = 0;

  void copyNodes(const LinkedList &other) {
    Node **nextLink = &head;

    try {
      for (Node *current = other.head; current != nullptr;
           current = current->next) {
        *nextLink = new Node(current->data);
        nextLink = &((*nextLink)->next);
        count++;
      }
    } catch (...) {
      clear();
      throw;
    }
  }

public:
  LinkedList() = default;

  // Copy constructor: creates an independent copy.
  LinkedList(const LinkedList &other) { copyNodes(other); }

  // Copy assignment: safely replaces the current list.
  LinkedList &operator=(const LinkedList &other) {
    if (this != &other) {
      LinkedList copy(other);
      std::swap(head, copy.head);
      std::swap(count, copy.count);
    }

    return *this;
  }

  ~LinkedList() { clear(); }

  void pushFront(const T &item) {
    Node *newNode = new Node(item);
    newNode->next = head;
    head = newNode;
    count++;
  }

  T popFront() {
    if (isEmpty()) {
      throw std::underflow_error("Linked list is empty.");
    }

    Node *oldHead = head;
    T item = oldHead->data;

    head = head->next;
    delete oldHead;
    count--;

    return item;
  }

  T removeAt(int index) {
    if (index < 0 || index >= count) {
      throw std::out_of_range("Invalid linked list index.");
    }

    if (index == 0) {
      return popFront();
    }

    Node *previous = head;

    for (int i = 0; i < index - 1; i++) {
      previous = previous->next;
    }

    Node *target = previous->next;
    T item = target->data;

    previous->next = target->next;
    delete target;
    count--;

    return item;
  }

  T &front() {
    if (isEmpty()) {
      throw std::underflow_error("Linked list is empty.");
    }

    return head->data;
  }

  const T &front() const {
    if (isEmpty()) {
      throw std::underflow_error("Linked list is empty.");
    }

    return head->data;
  }

  T &getAt(int index) {
    if (index < 0 || index >= count) {
      throw std::out_of_range("Invalid linked list index.");
    }

    Node *current = head;

    for (int i = 0; i < index; i++) {
      current = current->next;
    }

    return current->data;
  }

  int size() const { return count; }

  bool isEmpty() const { return head == nullptr; }

  void clear() {
    while (head != nullptr) {
      Node *oldHead = head;
      head = head->next;
      delete oldHead;
    }

    count = 0;
  }
};

#endif
