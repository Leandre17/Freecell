#include "game.hpp"
#include <iostream>

bool FreecellGame::isWon() const {
    for (const auto& foundation : board.foundations) {
        if (foundation.size() != 13)
            return false;
    }
    return true;
}

void FreecellGame::undoMove() {
    if (boardHistory.empty() || boardHistory.size() <= 1) {
        std::cout << "No moves to undo.\n";
        return;
    }
    board = boardHistory.back();
    boardHistory.pop_back();
    std::cout << "Move undone.\n";
}

void FreecellGame::undoMoves(int n) {
    if (n <= 0) {
        std::cout << "Number of moves to undo must be positive.\n";
        return;
    }
    for (int i = 0; i < n; ++i) {
        undoMove();
    }
}

void FreecellGame::revert() {
    if (boardHistory.empty()) {
        std::cout << "No initial state to revert to.\n";
        return;
    }
    board = boardHistory.front();
    boardHistory.clear();
    boardHistory.push_back(board);
    moveHistory.clear();
    std::cout << "Reverted to the initial game state.\n";
    display();
}
