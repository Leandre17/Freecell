#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "card.hpp"
using namespace std;
// Game state
class FreecellGame {
private:
  vector<vector<Card>> tableau;     // 8 columns
  vector<Card> freecells;           // 4 free cells
  vector<vector<Card>> foundations; // 4 foundation piles

  void initializeDeck(vector<Card> &deck);
  void shuffleDeck(vector<Card> &deck);
  void dealCards(vector<Card> &deck);
  void moveCard(const string &from, const string &to);
  bool canMoveToFoundation(const Card &card, int foundationIndex) const;
  bool canMoveToTableau(const Card &card, int tableauIndex) const;
  int howManyCanMove() const;
  bool isWon() const;
  void autoMoveToFoundation();
  void autoMoveOneCard(const string &from);
  void moveSequence(const string &from, const string &to, int count);

public:
  FreecellGame();
  void newGame();
  void display() const;
  void gameLoop();
};
