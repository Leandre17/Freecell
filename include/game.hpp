#pragma once
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

/**
 * @file game.hpp
 * @brief High-level game logic and interactive loop for Freecell.
 */

/**
 * @class FreecellGame
 * @brief Encapsulates game state, history and the main game loop.
 */
class FreecellGame {
  private:
    FreecellBoard board;
    list<string> moveHistory;
    list<FreecellBoard> boardHistory;
    // Allow SaveManager to access private members for save/load
    friend class SaveManager;
    bool isWon() const;
    void handleMoveCommands(const string& command);
    void showHelp() const;
    void revert();

  public:
    FreecellGame();
    /** Start a new game (reset board and history). */
    void newGame();

    /** Display the current board to stdout. */
    void display() const;

    /** Run the interactive game loop, reading commands from stdin. */
    void gameLoop();

    /** Undo the last move. */
    void undoMove();

    /** Undo the last @p n moves. */
    void undoMoves(int n);
};
