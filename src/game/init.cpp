#include "game.hpp"

FreecellGame::FreecellGame() {
    board = FreecellBoard();
    newGame();
}

void FreecellGame::newGame() {
    // Clear all data structures
    for (auto& col : board.tableau)
        col.clear();
    for (auto& foundation : board.foundations)
        foundation.clear();
    for (Card& cell : board.freecells)
        cell = Card{' ', 0};

    // Create and shuffle deck
    vector<Card> deck;
    board.initializeDeck(deck);
    board.shuffleDeck(deck);
    board.dealCards(deck);
    boardHistory.clear();
    boardHistory.push_back(board);
    moveHistory.clear();
}
