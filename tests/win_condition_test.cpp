#include "GameBoard.h"

#include <cassert>
#include <iostream>

void testNewGameIsNotWon() {
  GameBoard board;
  board.initialize();

  assert(!board.hasWon());

  for (int suit = 0; suit < 4; suit++) {
    assert(board.getFoundationSize(suit) == 0);
  }
}

void testIncompleteFoundationsAreNotWon() {
  GameBoard board;
  board.initialize();

  // Drawing and recycling cards must not count as winning.
  while (board.getStockSize() > 0) {
    board.drawFromStock();
  }

  assert(!board.hasWon());

  board.recycleWaste();

  assert(!board.hasWon());
}

void testRestartResetsWinCondition() {
  GameBoard board;
  board.initialize();

  while (board.getStockSize() > 0) {
    board.drawFromStock();
  }

  board.recycleWaste();
  board.initialize();

  assert(!board.hasWon());
}

int main() {
  testNewGameIsNotWon();
  testIncompleteFoundationsAreNotWon();
  testRestartResetsWinCondition();

  std::cout << "All win condition tests passed!\n";

  return 0;
}
