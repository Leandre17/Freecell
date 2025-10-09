#include "board.hpp"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>
using namespace std;
// Game state
class FreecellGame {
private:
  FreecellBoard board;
  bool isWon() const;

public:
  FreecellGame();
  void newGame();
  void display() const;
  void gameLoop();
};
