#include "GameBoard.h"

#include <iostream>
#include <stdexcept>

int GameBoard::getCardId(const Card &card) const {
  int suit = static_cast<int>(card.getSuit());
  int rank = static_cast<int>(card.getRank());

  return suit * 13 + (rank - 1);
}

void GameBoard::initialize() {
  // Reset all piles.
  for (int i = 0; i < 7; i++) {
    tableau[i].clear();
  }

  for (int i = 0; i < 4; i++) {
    foundations[i].clear();
  }

  waste.clear();
  stock.clear();

  Deck deck;
  deck.shuffle();

  // Deal cards into the seven tableau columns.
  for (int column = 0; column < 7; column++) {
    for (int row = 0; row <= column; row++) {
      Card card = deck.draw();

      if (row == column) {
        card.flip();
      }

      tableau[column].pushFront(card);

      CardLocation location;
      location.pile = PileType::Tableau;
      location.pileIndex = column;
      location.position = column - row;
      location.faceUp = card.isFaceUp();

      cardLocations.insert(getCardId(card), location);
    }
  }

  // Put the remaining cards into the stock.
  while (!deck.isEmpty()) {
    Card card = deck.draw();
    stock.enqueue(card);

    CardLocation location;
    location.pile = PileType::Stock;
    location.pileIndex = 0;
    location.position = stock.size() - 1;
    location.faceUp = false;

    cardLocations.insert(getCardId(card), location);
  }
}

int GameBoard::getTableauSize(int column) const {
  if (column < 0 || column >= 7) {
    throw std::out_of_range("Invalid tableau column.");
  }

  return tableau[column].size();
}

Card &GameBoard::getTableauCard(int column, int position) {
  if (column < 0 || column >= 7) {
    throw std::out_of_range("Invalid tableau column.");
  }

  return tableau[column].getAt(position);
}

int GameBoard::getStockSize() const { return stock.size(); }

int GameBoard::getWasteSize() const { return waste.size(); }

int GameBoard::getFoundationSize(int suit) const {
  if (suit < 0 || suit >= 4) {
    throw std::out_of_range("Invalid foundation pile.");
  }

  return foundations[suit].size();
}

CardLocation *GameBoard::getCardLocation(int cardId) {
  return cardLocations.search(cardId);
}

void GameBoard::display() {
  std::cout << "\n========== KLONDIKE SOLITAIRE ==========\n\n";

  std::cout << "STOCK: " << stock.size() << " cards";
  std::cout << "        WASTE: ";

  if (waste.isEmpty()) {
    std::cout << "--";
  } else {
    Card &card = waste.top();

    std::cout << static_cast<int>(card.getRank()) << " of suit "
              << static_cast<int>(card.getSuit());
  }

  std::cout << "\n\nFOUNDATIONS:\n";

  for (int i = 0; i < 4; i++) {
    std::cout << "  Suit " << i << ": " << foundations[i].size() << " cards\n";
  }

  std::cout << "\nTABLEAU:\n";

  for (int column = 0; column < 7; column++) {
    std::cout << "Column " << column + 1 << ": ";

    for (int position = 0; position < tableau[column].size(); position++) {

      Card &card = tableau[column].getAt(position);

      if (!card.isFaceUp()) {
        std::cout << "[##] ";
      } else {
        std::cout << "[" << static_cast<int>(card.getRank()) << "-"
                  << static_cast<int>(card.getSuit()) << "] ";
      }
    }

    std::cout << "\n";
  }

  std::cout << "\n========================================\n";
}
