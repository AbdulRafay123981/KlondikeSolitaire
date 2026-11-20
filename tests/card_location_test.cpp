#include "GameBoard.h"
#include "GameHistory.h"

#include <cassert>
#include <iostream>

void testInitialLocations() {
  GameBoard board;

  board.initialize();
  assert(board.validateCardLocations());

  // Reinitializing must replace all previous card locations.
  board.initialize();
  assert(board.validateCardLocations());
}

void testDrawAndRecycleLocations() {
  GameBoard board;
  board.initialize();

  assert(board.drawFromStock(3));
  assert(board.validateCardLocations());

  assert(board.drawFromStock(1));
  assert(board.validateCardLocations());

  // Empty the remaining stock in draw-three mode.
  while (board.getStockSize() > 0) {
    assert(board.drawFromStock(3));
    assert(board.validateCardLocations());
  }

  assert(board.getWasteSize() == 24);

  assert(board.recycleWaste());
  assert(board.validateCardLocations());

  assert(board.getStockSize() == 24);
  assert(board.getWasteSize() == 0);
}

void testUndoRedoLocations() {
  GameBoard board;
  GameHistory history;

  board.initialize();
  assert(board.validateCardLocations());

  GameBoard previousState = board;

  assert(board.drawFromStock(3));
  history.record(previousState);

  assert(board.validateCardLocations());

  assert(history.undo(board));
  assert(board.validateCardLocations());

  assert(history.redo(board));
  assert(board.validateCardLocations());
}

void testTableauMovementLocations() {
  GameBoard board;
  bool foundMove = false;

  for (int attempt = 0; attempt < 1000 && !foundMove; attempt++) {
    board.initialize();
    assert(board.validateCardLocations());

    for (int source = 0; source < 7 && !foundMove; source++) {
      for (int position = 0;
           position < board.getTableauSize(source) && !foundMove; position++) {

        if (!board.getTableauCard(source, position).isFaceUp()) {
          continue;
        }

        for (int destination = 0; destination < 7 && !foundMove;
             destination++) {

          if (source == destination) {
            continue;
          }

          if (board.moveTableauSequence(source, position, destination)) {
            foundMove = true;
            assert(board.validateCardLocations());
          }
        }
      }
    }
  }

  assert(foundMove);
}

void testFoundationMovementLocations() {
  GameBoard board;
  bool foundMove = false;

  for (int attempt = 0; attempt < 1000 && !foundMove; attempt++) {
    board.initialize();

    for (int column = 0; column < 7 && !foundMove; column++) {
      if (board.getTableauSize(column) == 0) {
        continue;
      }

      if (board.moveTableauToFoundation(column)) {
        foundMove = true;
        assert(board.validateCardLocations());
      }
    }
  }

  assert(foundMove);
}

int main() {
  testInitialLocations();
  testDrawAndRecycleLocations();
  testUndoRedoLocations();
  testTableauMovementLocations();
  testFoundationMovementLocations();

  std::cout << "All card location consistency tests passed!\n";

  return 0;
}
