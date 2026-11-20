#include "GameBoard.h"

#include <iostream>
#include <stdexcept>

namespace {
bool isRed(const Card &card) {
  return card.getSuit() == Suit::Hearts || card.getSuit() == Suit::Diamonds;
}

bool isValidDescendingSequence(const Card &upper, const Card &lower) {
  int upperRank = static_cast<int>(upper.getRank());
  int lowerRank = static_cast<int>(lower.getRank());

  return upperRank == lowerRank + 1 && isRed(upper) != isRed(lower);
}
} // namespace

int GameBoard::getCardId(const Card &card) const {
  int suit = static_cast<int>(card.getSuit());
  int rank = static_cast<int>(card.getRank());

  return suit * 13 + (rank - 1);
}

void GameBoard::initialize() {
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

void GameBoard::updateStockLocations() {
  for (int i = 0; i < stock.size(); i++) {
    Card &card = stock.at(i);
    CardLocation *location = cardLocations.search(getCardId(card));

    if (location != nullptr) {
      location->pile = PileType::Stock;
      location->pileIndex = 0;
      location->position = i;
      location->faceUp = card.isFaceUp();
    }
  }
}

void GameBoard::updateTableauLocations(int column) {
  for (int i = 0; i < tableau[column].size(); i++) {
    Card &card = tableau[column].getAt(i);
    CardLocation *location = cardLocations.search(getCardId(card));

    if (location != nullptr) {
      location->pile = PileType::Tableau;
      location->pileIndex = column;
      location->position = i;
      location->faceUp = card.isFaceUp();
    }
  }
}

bool GameBoard::drawFromStock() {
  if (stock.isEmpty()) {
    return false;
  }

  Card card = stock.dequeue();

  if (!card.isFaceUp()) {
    card.flip();
  }

  waste.push(card);

  CardLocation *location = cardLocations.search(getCardId(card));

  if (location != nullptr) {
    location->pile = PileType::Waste;
    location->pileIndex = 0;
    location->position = waste.size() - 1;
    location->faceUp = true;
  }

  updateStockLocations();

  return true;
}

bool GameBoard::recycleWaste() {
  if (!stock.isEmpty() || waste.isEmpty()) {
    return false;
  }

  Stack<Card> temporary;

  while (!waste.isEmpty()) {
    temporary.push(waste.pop());
  }

  while (!temporary.isEmpty()) {
    Card card = temporary.pop();

    if (card.isFaceUp()) {
      card.flip();
    }

    stock.enqueue(card);

    CardLocation *location = cardLocations.search(getCardId(card));

    if (location != nullptr) {
      location->pile = PileType::Stock;
      location->pileIndex = 0;
      location->position = stock.size() - 1;
      location->faceUp = false;
    }
  }

  return true;
}

bool GameBoard::moveTableauSequence(int sourceColumn, int sourcePosition,
                                    int destinationColumn) {
  if (sourceColumn < 0 || sourceColumn >= 7 || destinationColumn < 0 ||
      destinationColumn >= 7 || sourceColumn == destinationColumn) {
    return false;
  }

  if (sourcePosition < 0 || sourcePosition >= tableau[sourceColumn].size()) {
    return false;
  }

  Card &firstCard = tableau[sourceColumn].getAt(sourcePosition);

  if (!firstCard.isFaceUp()) {
    return false;
  }

  // Count the consecutive face-up cards being moved.
  int sequenceLength = 1;

  while (sourcePosition + sequenceLength < tableau[sourceColumn].size()) {

    Card &upper =
        tableau[sourceColumn].getAt(sourcePosition + sequenceLength - 1);

    Card &lower = tableau[sourceColumn].getAt(sourcePosition + sequenceLength);

    if (!lower.isFaceUp()) {
      break;
    }

    if (!isValidDescendingSequence(upper, lower)) {
      return false;
    }

    sequenceLength++;
  }

  // Check whether the destination accepts the first card.
  if (tableau[destinationColumn].isEmpty()) {
    if (firstCard.getRank() != Rank::King) {
      return false;
    }
  } else {
    Card &destinationTop = tableau[destinationColumn].getAt(0);

    if (!destinationTop.isFaceUp() ||
        !isValidDescendingSequence(destinationTop, firstCard)) {
      return false;
    }
  }

  // Extract the selected sequence.
  // The temporary stack reverses it during extraction.
  Stack<Card> temporary;

  for (int i = 0; i < sequenceLength; i++) {
    temporary.push(tableau[sourceColumn].removeAt(sourcePosition));
  }

  // Restore the original sequence order at the destination.
  while (!temporary.isEmpty()) {
    tableau[destinationColumn].pushFront(temporary.pop());
  }

  // If the first card of the source column was moved,
  // reveal the newly exposed card.
  if (sourcePosition == 0 && !tableau[sourceColumn].isEmpty()) {
    Card &exposedCard = tableau[sourceColumn].getAt(0);

    if (!exposedCard.isFaceUp()) {
      exposedCard.flip();
    }
  }

  updateTableauLocations(sourceColumn);
  updateTableauLocations(destinationColumn);

  return true;
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

Card *GameBoard::getWasteTop() {
  if (waste.isEmpty()) {
    return nullptr;
  }

  return &waste.top();
}

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

  Card *wasteCard = getWasteTop();

  if (wasteCard == nullptr) {
    std::cout << "--";
  } else {
    std::cout << static_cast<int>(wasteCard->getRank()) << " of suit "
              << static_cast<int>(wasteCard->getSuit());
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
