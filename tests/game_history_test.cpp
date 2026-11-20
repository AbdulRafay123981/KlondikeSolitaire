#include "GameHistory.h"

#include <cassert>
#include <iostream>

void testUndoRestoresPreviousState() {
  GameBoard board;
  board.initialize();

  GameHistory history;

  int originalStock = board.getStockSize();

  GameBoard previousState = board;

  assert(board.drawFromStock());
  assert(board.getStockSize() == originalStock - 1);

  history.record(previousState);

  assert(history.canUndo());
  assert(history.undo(board));

  assert(board.getStockSize() == originalStock);
  assert(board.getWasteSize() == 0);
  assert(!history.canUndo());
  assert(history.canRedo());
}

void testRedoRestoresUndoneState() {
  GameBoard board;
  board.initialize();

  GameHistory history;

  GameBoard previousState = board;

  assert(board.drawFromStock());
  history.record(previousState);

  int stockAfterDraw = board.getStockSize();

  assert(history.undo(board));
  assert(board.getStockSize() == 24);

  assert(history.redo(board));
  assert(board.getStockSize() == stockAfterDraw);
  assert(board.getWasteSize() == 1);

  assert(!history.canRedo());
  assert(history.canUndo());
}

void testNewMoveClearsRedoHistory() {
  GameBoard board;
  board.initialize();

  GameHistory history;

  GameBoard firstState = board;

  assert(board.drawFromStock());
  history.record(firstState);

  assert(history.undo(board));
  assert(history.canRedo());

  GameBoard secondState = board;

  assert(board.drawFromStock());
  history.record(secondState);

  assert(!history.canRedo());
  assert(history.canUndo());
}

void testUndoAndRedoWithoutHistory() {
  GameBoard board;
  board.initialize();

  GameHistory history;

  assert(!history.undo(board));
  assert(!history.redo(board));
}

void testClearHistory() {
  GameBoard board;
  board.initialize();

  GameHistory history;

  GameBoard previousState = board;

  assert(board.drawFromStock());
  history.record(previousState);

  assert(history.canUndo());

  history.clear();

  assert(!history.canUndo());
  assert(!history.canRedo());
}

int main() {
  testUndoRestoresPreviousState();
  testRedoRestoresUndoneState();
  testNewMoveClearsRedoHistory();
  testUndoAndRedoWithoutHistory();
  testClearHistory();

  std::cout << "All Game History tests passed!\n";

  return 0;
}
