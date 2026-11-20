#include "Deck.h"

#include <iostream>
#include <stdexcept>

int main() {
  Deck deck;

  // Track every suit-rank combination.
  bool found[4][14] = {};

  for (int i = 0; i < 52; ++i) {
    Card card = deck.draw();

    int suit = static_cast<int>(card.getSuit());
    int rank = static_cast<int>(card.getRank());

    if (suit < 0 || suit >= 4 || rank < 1 || rank > 13) {
      std::cout << "FAIL: Invalid card found.\n";
      return 1;
    }

    if (found[suit][rank]) {
      std::cout << "FAIL: Duplicate card found.\n";
      return 1;
    }

    found[suit][rank] = true;
  }

  // The deck should now be empty.
  if (!deck.isEmpty()) {
    std::cout << "FAIL: Deck should be empty after 52 draws.\n";
    return 1;
  }

  // Drawing from an empty deck should throw.
  try {
    deck.draw();
    std::cout << "FAIL: Drawing from an empty deck did not throw.\n";
    return 1;
  } catch (const std::out_of_range &) {
    std::cout << "PASS: Empty deck throws an exception.\n";
  }

  // Shuffling should reset the deck.
  deck.shuffle();

  if (deck.isEmpty()) {
    std::cout << "FAIL: Shuffling did not reset the deck.\n";
    return 1;
  }

  for (int i = 0; i < 52; ++i) {
    deck.draw();
  }

  if (!deck.isEmpty()) {
    std::cout << "FAIL: Deck should be empty after drawing again.\n";
    return 1;
  }

  std::cout << "PASS: All 52 cards are unique.\n";
  std::cout << "PASS: Deck becomes empty after 52 draws.\n";
  std::cout << "PASS: Shuffling resets the deck.\n";
  std::cout << "All deck tests passed!\n";

  return 0;
}
