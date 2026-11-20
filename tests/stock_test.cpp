#include "GameBoard.h"

#include <cassert>
#include <iostream>

int main() {
  GameBoard board;
  board.initialize();

  assert(board.getStockSize() == 24);
  assert(board.getWasteSize() == 0);

  // Drawing from an empty waste should initially fail.
  assert(board.getWasteTop() == nullptr);

  // Draw the first card and remember it.
  assert(board.drawFromStock());

  Card *firstDrawn = board.getWasteTop();

  assert(firstDrawn != nullptr);
  assert(firstDrawn->isFaceUp());

  int firstSuit = static_cast<int>(firstDrawn->getSuit());
  int firstRank = static_cast<int>(firstDrawn->getRank());
  int firstId = firstSuit * 13 + firstRank - 1;

  assert(board.getStockSize() == 23);
  assert(board.getWasteSize() == 1);

  CardLocation *firstLocation = board.getCardLocation(firstId);

  assert(firstLocation != nullptr);
  assert(firstLocation->pile == PileType::Waste);
  assert(firstLocation->position == 0);
  assert(firstLocation->faceUp);

  // Draw the remaining 23 cards.
  for (int i = 0; i < 23; i++) {
    assert(board.drawFromStock());
  }

  assert(board.getStockSize() == 0);
  assert(board.getWasteSize() == 24);

  // Cannot draw when stock is empty.
  assert(!board.drawFromStock());

  // Recycle the waste into stock.
  assert(board.recycleWaste());

  assert(board.getStockSize() == 24);
  assert(board.getWasteSize() == 0);
  assert(board.getWasteTop() == nullptr);

  // The recycled cards should be face-down.
  CardLocation *recycledLocation = board.getCardLocation(firstId);

  assert(recycledLocation != nullptr);
  assert(recycledLocation->pile == PileType::Stock);
  assert(!recycledLocation->faceUp);

  // The original first-drawn card should be drawn first again.
  assert(board.drawFromStock());

  Card *recycledFirst = board.getWasteTop();

  assert(recycledFirst != nullptr);
  assert(static_cast<int>(recycledFirst->getSuit()) == firstSuit);
  assert(static_cast<int>(recycledFirst->getRank()) == firstRank);
  assert(recycledFirst->isFaceUp());

  // Recycling should fail when stock is not empty.
  assert(!board.recycleWaste());

  std::cout << "All stock and waste tests passed!\n";

  return 0;
}
