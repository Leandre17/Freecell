#include "game.hpp"
#include <iostream>

void FreecellGame::display() const {
    std::cout << "\n=== Freecell ===\n";

    // Display freecells
    std::cout << "Free Cells: ";
    for (const Card& cell : board.freecells) {
        if (cell.rank != 0) {
            std::cout << "[" << cell.toString() << "] ";
        } else {
            std::cout << "[ ] ";
        }
    }
    std::cout << "\n";

    // Display foundations
    std::cout << "Foundations: ";
    for (const auto& foundation : board.foundations) {
        if (!foundation.empty()) {
            std::cout << "[" << foundation.back().toString() << "] ";
        } else {
            std::cout << "[ ] ";
        }
    }
    std::cout << "\n";

    // Display tableau
    for (size_t i = 0; i < board.tableau.size(); i++) {
        std::cout << "T" << (i + 1) << "\t";
    }
    std::cout << "\n";

    size_t maxHeight = 0;
    for (const auto& col : board.tableau) {
        maxHeight = std::max(maxHeight, col.size());
    }

    for (size_t row = 0; row < maxHeight; ++row) {
        for (size_t col = 0; col < 8; ++col) {
            if (row < board.tableau[col].size()) {
                std::cout << board.tableau[col][row].toString() << "\t";
            } else {
                std::cout << "\t";
            }
        }
        std::cout << "\n";
    }
}
