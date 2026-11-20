#include "GameBoard.h"

#include <cassert>
#include <iostream>

void testDrawOne() {
  GameBoard board;
  board.initialize();

  assert(board.drawFromStock());

  assert(board.getStockSize() == 23);
  assert(board.getWasteSize() == 1);
  assert(board.getWasteTop() != nullptr);
  assert(board.getWasteTop()->isFaceUp());
}

void testDrawThree() {
  GameBoard board;
  board.initialize();

  assert(board.drawFromStock(3));

  assert(board.getStockSize() == 21);
  assert(board.getWasteSize() == 3);
  assert(board.getWasteTop() != nullptr);
  assert(board.getWasteTop()->isFaceUp());
}

void testFinalPartialDraw() {
  GameBoard board;
  board.initialize();

  for (int i = 0; i < 7; i++) {
    assert(board.drawFromStock(3));
  }

  assert(board.getStockSize() == 3);
  assert(board.getWasteSize() == 21);

  // Only three cards remain, so the final draw succeeds.
  assert(board.drawFromStock(3));

  assert(board.getStockSize() == 0);
  assert(board.getWasteSize() == 24);

  assert(!board.drawFromStock(3));
}

void testRecycleAfterDrawThree() {
  GameBoard board;
  board.initialize();

  while (board.getStockSize() > 0) {
    assert(board.drawFromStock(3));
  }

  assert(board.getWasteSize() == 24);

  assert(board.recycleWaste());

  assert(board.getStockSize() == 24);
  assert(board.getWasteSize() == 0);

  assert(board.drawFromStock(3));

  assert(board.getStockSize() == 21);
  assert(board.getWasteSize() == 3);
}

void testInvalidDrawCount() {
  GameBoard board;
  board.initialize();

  assert(!board.drawFromStock(2));
  assert(!board.drawFromStock(0));
  assert(board.getStockSize() == 24);
  assert(board.getWasteSize() == 0);
}

int main() {
  testDrawOne();
  testDrawThree();
  testFinalPartialDraw();
  testRecycleAfterDrawThree();
  testInvalidDrawCount();

  std::cout << "All draw mode tests passed!\n";

  return 0;
}
