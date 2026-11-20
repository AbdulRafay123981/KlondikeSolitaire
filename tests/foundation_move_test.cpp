#include "GameBoard.h"

#include <cassert>
#include <iostream>

int getCardId(const Card &card) {
  return static_cast<int>(card.getSuit()) * 13 +
         static_cast<int>(card.getRank()) - 1;
}

void testTableauToFoundation() {
  GameBoard board;
  bool moveFound = false;

  for (int attempt = 0; attempt < 1000 && !moveFound; attempt++) {
    board.initialize();

    for (int column = 0; column < 7 && !moveFound; column++) {
      if (board.getTableauSize(column) == 0) {
        continue;
      }

      Card &card = board.getTableauCard(column, 0);

      if (card.getRank() != Rank::Ace) {
        continue;
      }

      int suit = static_cast<int>(card.getSuit());
      int cardId = getCardId(card);
      int oldSize = board.getTableauSize(column);

      assert(board.moveTableauToFoundation(column));

      moveFound = true;

      assert(board.getFoundationSize(suit) == 1);
      assert(board.getTableauSize(column) == oldSize - 1);

      CardLocation *location = board.getCardLocation(cardId);

      assert(location != nullptr);
      assert(location->pile == PileType::Foundation);
      assert(location->pileIndex == suit);
      assert(location->position == 0);
      assert(location->faceUp);

      if (board.getTableauSize(column) > 0) {
        assert(board.getTableauCard(column, 0).isFaceUp());
      }
    }
  }

  assert(moveFound);
}

void testWasteToFoundation() {
  GameBoard board;
  bool moveFound = false;

  for (int attempt = 0; attempt < 1000 && !moveFound; attempt++) {
    board.initialize();

    while (board.getStockSize() > 0 && !moveFound) {
      assert(board.drawFromStock());

      Card *card = board.getWasteTop();

      if (card->getRank() != Rank::Ace) {
        continue;
      }

      int suit = static_cast<int>(card->getSuit());
      int cardId = getCardId(*card);

      assert(board.moveWasteToFoundation());

      moveFound = true;

      assert(board.getFoundationSize(suit) == 1);

      CardLocation *location = board.getCardLocation(cardId);

      assert(location != nullptr);
      assert(location->pile == PileType::Foundation);
      assert(location->pileIndex == suit);
      assert(location->position == 0);
      assert(location->faceUp);
    }
  }

  assert(moveFound);
}

void testWasteToTableau() {
  GameBoard board;
  bool moveFound = false;

  for (int attempt = 0; attempt < 1000 && !moveFound; attempt++) {
    board.initialize();

    while (board.getStockSize() > 0 && !moveFound) {
      assert(board.drawFromStock());

      Card *card = board.getWasteTop();
      int cardId = getCardId(*card);

      for (int destination = 0; destination < 7 && !moveFound; destination++) {
        int oldSize = board.getTableauSize(destination);

        if (board.moveWasteToTableau(destination)) {
          moveFound = true;

          assert(board.getTableauSize(destination) == oldSize + 1);

          CardLocation *location = board.getCardLocation(cardId);

          assert(location != nullptr);
          assert(location->pile == PileType::Tableau);
          assert(location->pileIndex == destination);
          assert(location->position == 0);
          assert(location->faceUp);

          assert(board.getWasteSize() < 24);
        }
      }
    }
  }

  assert(moveFound);
}

int main() {
  testTableauToFoundation();
  testWasteToFoundation();
  testWasteToTableau();

  std::cout << "All foundation and waste movement tests passed!\n";

  return 0;
}
