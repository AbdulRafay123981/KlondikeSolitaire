#include "GameBoard.h"

#include <cassert>
#include <iostream>

int main() {
  GameBoard board;
  bool foundMove = false;

  // A shuffled deal may or may not have an immediately
  // available tableau-to-tableau move.
  for (int attempt = 0; attempt < 1000 && !foundMove; attempt++) {
    board.initialize();

    for (int source = 0; source < 7 && !foundMove; source++) {
      for (int position = 0;
           position < board.getTableauSize(source) && !foundMove; position++) {

        Card &candidate = board.getTableauCard(source, position);

        if (!candidate.isFaceUp()) {
          continue;
        }

        int cardId = static_cast<int>(candidate.getSuit()) * 13 +
                     static_cast<int>(candidate.getRank()) - 1;

        for (int destination = 0; destination < 7 && !foundMove;
             destination++) {

          if (source == destination) {
            continue;
          }

          int sourceSize = board.getTableauSize(source);
          int destinationSize = board.getTableauSize(destination);

          if (board.moveTableauSequence(source, position, destination)) {

            foundMove = true;

            assert(board.getTableauSize(source) < sourceSize);
            assert(board.getTableauSize(destination) > destinationSize);

            CardLocation *location = board.getCardLocation(cardId);

            assert(location != nullptr);
            assert(location->pile == PileType::Tableau);
            assert(location->pileIndex == destination);
            assert(location->position == 0);
            assert(location->faceUp);

            // Moving the visible card should reveal the next card.
            if (sourceSize > 2) {
              assert(board.getTableauCard(source, 1).isFaceUp());
            }

            // All cards must still be accounted for.
            int total = board.getStockSize() + board.getWasteSize();

            for (int column = 0; column < 7; column++) {
              total += board.getTableauSize(column);
            }

            for (int suit = 0; suit < 4; suit++) {
              total += board.getFoundationSize(suit);
            }

            assert(total == 52);
          }
        }
      }
    }
  }

  assert(foundMove);

  // A column cannot be moved onto itself.
  assert(!board.moveTableauSequence(0, 0, 0));

  std::cout << "All tableau movement tests passed!\n";

  return 0;
}
