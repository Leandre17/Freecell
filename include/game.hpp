#include "board.hpp"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <list>
#include <random>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
// Game state
class FreecellGame {
private:
  FreecellBoard board;
  list<string> moveHistory;
  list<FreecellBoard> boardHistory;
  bool isWon() const;
  void handleMoveCommands(const string &command);
  void showHelp() const;
  void revert();

public:
  FreecellGame();
  void newGame();
  void display() const;
  void gameLoop();
  void undoMove();
  void undoMoves(int n);
};
