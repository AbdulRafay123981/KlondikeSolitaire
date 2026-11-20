#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Card.h"
#include "Deck.h"
#include "HashMap.h"
#include "LinkedList.h"
#include "Queue.h"
#include "Stack.h"

enum class PileType { Tableau, Stock, Waste, Foundation };

struct CardLocation {
  PileType pile = PileType::Stock;
  int pileIndex = 0;
  int position = 0;
  bool faceUp = false;
};

class GameBoard {
private:
  LinkedList<Card> tableau[7];
  Stack<Card> foundations[4];
  Stack<Card> waste;
  Queue<Card> stock;

  HashMap<CardLocation> cardLocations;

  int getCardId(const Card &card) const;
  void updateStockLocations();
  void updateTableauLocations(int column);

public:
  void initialize();
  void display();

  bool drawFromStock(int count = 1);
  bool recycleWaste();

  bool moveTableauSequence(int sourceColumn, int sourcePosition,
                           int destinationColumn);

  bool moveTableauToFoundation(int sourceColumn);
  bool moveWasteToTableau(int destinationColumn);
  bool moveWasteToFoundation();

  bool hasWon() const;

  int getTableauSize(int column) const;
  Card &getTableauCard(int column, int position);

  int getStockSize() const;
  int getWasteSize() const;
  Card *getWasteTop();

  int getFoundationSize(int suit) const;

  CardLocation *getCardLocation(int cardId);
};

#endif
