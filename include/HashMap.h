#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdexcept>

template <typename V, int Capacity = 101> class HashMap {
private:
  struct Entry {
    int key = -1;
    V value{};
    int state = 0; // 0 = empty, 1 = occupied, 2 = deleted
  };

  Entry table[Capacity];
  int count = 0;

  int hash1(int key) const { return key % Capacity; }

  int hash2(int key) const { return 1 + (key % (Capacity - 2)); }

public:
  HashMap() {
    static_assert(Capacity > 2, "HashMap capacity must be greater than 2.");
  }

  void insert(int key, const V &value) {
    int index = hash1(key);
    int step = hash2(key);
    int deletedIndex = -1;

    for (int i = 0; i < Capacity; i++) {
      int current = (index + i * step) % Capacity;

      if (table[current].state == 1) {
        if (table[current].key == key) {
          table[current].value = value;
          return;
        }
      } else if (table[current].state == 2) {
        if (deletedIndex == -1) {
          deletedIndex = current;
        }
      } else {
        int target = (deletedIndex == -1) ? current : deletedIndex;

        table[target].key = key;
        table[target].value = value;
        table[target].state = 1;
        count++;
        return;
      }
    }

    if (deletedIndex != -1) {
      table[deletedIndex].key = key;
      table[deletedIndex].value = value;
      table[deletedIndex].state = 1;
      count++;
      return;
    }

    throw std::overflow_error("HashMap is full.");
  }

  V *search(int key) {
    int index = hash1(key);
    int step = hash2(key);

    for (int i = 0; i < Capacity; i++) {
      int current = (index + i * step) % Capacity;

      if (table[current].state == 0) {
        return nullptr;
      }

      if (table[current].state == 1 && table[current].key == key) {
        return &table[current].value;
      }
    }

    return nullptr;
  }

  const V *search(int key) const {
    int index = hash1(key);
    int step = hash2(key);

    for (int i = 0; i < Capacity; i++) {
      int current = (index + i * step) % Capacity;

      if (table[current].state == 0) {
        return nullptr;
      }

      if (table[current].state == 1 && table[current].key == key) {
        return &table[current].value;
      }
    }

    return nullptr;
  }

  bool remove(int key) {
    int index = hash1(key);
    int step = hash2(key);

    for (int i = 0; i < Capacity; i++) {
      int current = (index + i * step) % Capacity;

      if (table[current].state == 0) {
        return false;
      }

      if (table[current].state == 1 && table[current].key == key) {
        table[current].state = 2;
        count--;
        return true;
      }
    }

    return false;
  }

  bool contains(int key) { return search(key) != nullptr; }

  int size() const { return count; }

  bool isEmpty() const { return count == 0; }
};

#endif
