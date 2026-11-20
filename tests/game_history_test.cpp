#include "GameHistory.h"

#include <cassert>
#include <iostream>

int getCardId(const Card &card) {
  return static_cast<int>(card.getSuit()) * 13 +
         static_cast<int>(card.getRank()) - 1;
}

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

void testTableauMoveUndoRedo() {
  GameBoard board;
  GameHistory history;

  bool foundMove = false;

  for (int attempt = 0; attempt < 1000 && !foundMove; attempt++) {
    board.initialize();
    history.clear();

    for (int source = 0; source < 7 && !foundMove; source++) {
      // We need a hidden card underneath the visible card.
      if (board.getTableauSize(source) < 2) {
        continue;
      }

      if (!board.getTableauCard(source, 0).isFaceUp() ||
          board.getTableauCard(source, 1).isFaceUp()) {
        continue;
      }

      for (int destination = 0; destination < 7 && !foundMove; destination++) {
        if (source == destination) {
          continue;
        }

        GameBoard previousState = board;

        int sourceSize = board.getTableauSize(source);
        int destinationSize = board.getTableauSize(destination);

        int movedCardId = getCardId(board.getTableauCard(source, 0));

        if (!board.moveTableauSequence(source, 0, destination)) {
          continue;
        }

        foundMove = true;
        history.record(previousState);

        // The hidden card should now be revealed.
        assert(board.getTableauSize(source) == sourceSize - 1);
        assert(board.getTableauSize(destination) == destinationSize + 1);
        assert(board.getTableauCard(source, 0).isFaceUp());
        assert(board.validateCardLocations());

        // Undo should restore the original piles and face states.
        assert(history.undo(board));

        assert(board.getTableauSize(source) == sourceSize);
        assert(board.getTableauSize(destination) == destinationSize);
        assert(board.getTableauCard(source, 0).isFaceUp());
        assert(!board.getTableauCard(source, 1).isFaceUp());
        assert(getCardId(board.getTableauCard(source, 0)) == movedCardId);
        assert(board.validateCardLocations());

        // Redo should restore the moved card and revealed card.
        assert(history.redo(board));

        assert(board.getTableauSize(source) == sourceSize - 1);
        assert(board.getTableauSize(destination) == destinationSize + 1);
        assert(board.getTableauCard(source, 0).isFaceUp());
        assert(board.validateCardLocations());

        CardLocation *location = board.getCardLocation(movedCardId);

        assert(location != nullptr);
        assert(location->pile == PileType::Tableau);
        assert(location->pileIndex == destination);
        assert(location->position == 0);
      }
    }
  }

  assert(foundMove);
}

void testWasteToFoundationUndoRedo() {
  GameBoard board;
  GameHistory history;

  bool foundMove = false;

  for (int attempt = 0; attempt < 1000 && !foundMove; attempt++) {
    board.initialize();
    history.clear();

    while (board.getStockSize() > 0 && !foundMove) {
      GameBoard drawState = board;

      assert(board.drawFromStock());
      history.record(drawState);

      Card *card = board.getWasteTop();

      if (card->getRank() != Rank::Ace) {
        continue;
      }

      int suit = static_cast<int>(card->getSuit());
      int cardId = getCardId(*card);
      int wasteSize = board.getWasteSize();

      GameBoard previousState = board;

      assert(board.moveWasteToFoundation());
      history.record(previousState);

      foundMove = true;

      assert(board.getWasteSize() == wasteSize - 1);
      assert(board.getFoundationSize(suit) == 1);
      assert(board.validateCardLocations());

      // Undo should return the Ace to the waste pile.
      assert(history.undo(board));

      assert(board.getWasteSize() == wasteSize);
      assert(board.getFoundationSize(suit) == 0);
      assert(board.getWasteTop() != nullptr);
      assert(getCardId(*board.getWasteTop()) == cardId);
      assert(board.validateCardLocations());

      // Redo should move the Ace back to its foundation.
      assert(history.redo(board));

      assert(board.getWasteSize() == wasteSize - 1);
      assert(board.getFoundationSize(suit) == 1);
      assert(board.validateCardLocations());

      CardLocation *location = board.getCardLocation(cardId);

      assert(location != nullptr);
      assert(location->pile == PileType::Foundation);
      assert(location->pileIndex == suit);
      assert(location->position == 0);
      assert(location->faceUp);
    }
  }

  assert(foundMove);
}

int main() {
  testUndoRestoresPreviousState();
  testRedoRestoresUndoneState();
  testNewMoveClearsRedoHistory();
  testUndoAndRedoWithoutHistory();
  testClearHistory();
  testTableauMoveUndoRedo();
  testWasteToFoundationUndoRedo();

  std::cout << "All Game History tests passed!\n";

  return 0;
}
