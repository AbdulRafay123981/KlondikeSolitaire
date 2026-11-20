#include "Deck.h"

#include <algorithm>
#include <random>
#include <stdexcept>

Deck::Deck() {
  int index = 0;

  for (int s = 0; s < 4; ++s) {
    for (int r = 1; r <= 13; ++r) {
      cards[index++] = Card(static_cast<Suit>(s), static_cast<Rank>(r));
    }
  }
}

void Deck::shuffle() {
  std::random_device rd;
  std::mt19937 generator(rd());

  std::shuffle(cards, cards + 52, generator);

  nextCard = 0;
}

Card Deck::draw() {
  if (isEmpty()) {
    throw std::out_of_range("Cannot draw from an empty deck.");
  }

  return cards[nextCard++];
}

bool Deck::isEmpty() const { return nextCard >= 52; }
