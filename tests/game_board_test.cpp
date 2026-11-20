#include "GameBoard.h"

#include <cassert>
#include <iostream>

int main() {
  GameBoard board;
  board.initialize();

  // Seven tableau columns.
  int totalTableauCards = 0;
  int faceUpCards = 0;

  for (int column = 0; column < 7; column++) {
    int expectedSize = column + 1;

    assert(board.getTableauSize(column) == expectedSize);
    totalTableauCards += board.getTableauSize(column);

    // Only the top card should be face-up.
    for (int position = 0; position < expectedSize; position++) {
      Card &card = board.getTableauCard(column, position);

      if (card.isFaceUp()) {
        faceUpCards++;
      }

      if (position == 0) {
        assert(card.isFaceUp());
      } else {
        assert(!card.isFaceUp());
      }
    }
  }

  assert(totalTableauCards == 28);
  assert(faceUpCards == 7);

  // Remaining cards should be in stock.
  assert(board.getStockSize() == 24);

  // Waste and foundations should initially be empty.
  assert(board.getWasteSize() == 0);

  for (int suit = 0; suit < 4; suit++) {
    assert(board.getFoundationSize(suit) == 0);
  }

  // Every card should have a location in the HashMap.
  for (int cardId = 0; cardId < 52; cardId++) {
    assert(board.getCardLocation(cardId) != nullptr);
  }

  // Check that all cards are accounted for.
  assert(totalTableauCards + board.getStockSize() == 52);

  std::cout << "All GameBoard tests passed!\n";

  return 0;
}
