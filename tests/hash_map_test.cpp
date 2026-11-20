#include "HashMap.h"

#include <cassert>
#include <iostream>

int main() {
  HashMap<int> map;

  // Empty map
  assert(map.isEmpty());
  assert(map.size() == 0);

  // Basic insertion
  map.insert(10, 100);
  map.insert(20, 200);

  assert(map.size() == 2);
  assert(*map.search(10) == 100);
  assert(*map.search(20) == 200);

  // Update existing key
  map.insert(10, 500);

  assert(*map.search(10) == 500);
  assert(map.size() == 2);

  // Double hashing collision test
  map.insert(5, 50);
  map.insert(106, 1060);

  assert(*map.search(5) == 50);
  assert(*map.search(106) == 1060);

  // Deletion
  assert(map.remove(5));
  assert(!map.contains(5));
  assert(map.contains(106));

  // Missing key
  assert(map.search(999) == nullptr);
  assert(!map.remove(999));

  // Reusing a deleted slot
  map.insert(207, 2070);

  assert(map.contains(207));
  assert(*map.search(207) == 2070);

  // Clear isn't implemented yet, so test remaining size
  assert(map.size() == 4);

  std::cout << "All HashMap tests passed!\n";

  return 0;
}
