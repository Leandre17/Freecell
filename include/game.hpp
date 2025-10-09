#include "board.hpp"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <list>
using namespace std;
// Game state
class FreecellGame {
private:
  FreecellBoard board;
  list<string> moveHistory;
  list<FreecellBoard> boardHistory;
  bool isWon() const;
  void handleMoveCommands(const string &command);

public:
  FreecellGame();
  void newGame();
  void display() const;
  void gameLoop();
  void undoMove();
  void undoMoves(int n);
};
