#pragma once
#include "card.hpp"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>
using namespace std;
// Game state
class FreecellBoard {
private:
public:
  vector<vector<Card>> tableau;     // 8 columns
  vector<Card> freecells;           // 4 free cells
  vector<vector<Card>> foundations; // 4 foundation piles
  FreecellBoard();
  bool operator!=(const FreecellBoard &b) const;
  void initializeDeck(vector<Card> &deck);
  void shuffleDeck(vector<Card> &deck);
  void dealCards(vector<Card> &deck);
  void moveCard(const string &from, const string &to);
  bool canMoveToFoundation(const Card &card, int foundationIndex) const;
  bool canMoveToTableau(const Card &card, int tableauIndex) const;
  int howManyCanMove() const;
  void autoMoveToFoundation();
  void autoMoveOneCard(const string &from);
  void moveSequence(const string &from, const string &to, int count);
  void display() const;
};

class InvalidMoveException : public std::exception {
public:
  const char *what() const noexcept override { return "Invalid move"; }
};