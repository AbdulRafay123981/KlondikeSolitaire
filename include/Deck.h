#ifndef DECK_H
#define DECK_H

#include "Card.h"

class Deck {
private:
  Card cards[52];
  int nextCard = 0;

public:
  Deck();

  void shuffle();
  Card draw();
  bool isEmpty() const;
};

#endif
