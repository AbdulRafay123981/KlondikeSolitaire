#ifndef CARD_H
#define CARD_H

enum class Suit { Hearts, Diamonds, Spades, Clubs };
enum class Rank {
  Ace,
  Two,
  Three,
  Four,
  Five,
  Six,
  Seven,
  Eight,
  Nine,
  Ten,
  Jack,
  Queen,
  King
};

class Card {
private:
  Suit suit;
  Rank rank;
  bool faceUp = false;

public:
  Card(Suit suit, Rank rank) : suit(suit), rank(rank) {}

  Suit getSuit() const { return suit; }
  Rank getRank() const { return rank; }
  bool isFaceUp() const { return faceUp; }

  void flip() { faceUp = !faceUp; }
};

#endif
