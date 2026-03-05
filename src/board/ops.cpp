#include "game.hpp"
using namespace std;

bool FreecellBoard::operator!=(const FreecellBoard& b) const {
    if (tableau != b.tableau)
        return true;
    if (freecells != b.freecells)
        return true;
    if (foundations != b.foundations)
        return true;
    return false;
}

int FreecellBoard::howManyCanMove() const {
    int emptyFreecells = 0;
    for (const Card& cell : freecells) {
        if (cell.rank == 0)
            ++emptyFreecells;
    }
    int emptyTableaus = 0;
    for (const auto& col : tableau) {
        if (col.empty())
            ++emptyTableaus;
    }
    return (emptyFreecells + 1) * (1 + emptyTableaus);
}

void FreecellBoard::autoMoveToFoundation() {
    bool moved;
    do {
        moved = false;
        for (size_t i = 0; i < tableau.size(); ++i) {
            if (!tableau[i].empty()) {
                Card cardToMove = tableau[i].back();
                for (size_t j = 0; j < foundations.size(); ++j) {
                    if (canMoveToFoundation(cardToMove, j)) {
                        foundations[j].push_back(cardToMove);
                        tableau[i].pop_back();
                        cout << "Auto-moved " << cardToMove.toString() << " to Foundation " << j + 1
                             << ".\n";
                        moved = true;
                        break;
                    }
                }
            }
        }
        for (size_t i = 0; i < freecells.size(); ++i) {
            if (freecells[i].rank != 0) {
                Card cardToMove = freecells[i];
                for (size_t j = 0; j < foundations.size(); ++j) {
                    if (canMoveToFoundation(cardToMove, j)) {
                        foundations[j].push_back(cardToMove);
                        freecells[i] = Card{' ', 0};
                        cout << "Auto-moved " << cardToMove.toString() << " to Foundation " << j + 1
                             << ".\n";
                        moved = true;
                        break;
                    }
                }
            }
        }
    } while (moved);
}

void FreecellBoard::autoMoveOneCard(const string& from) {
    if (from.empty()) {
        cout << "Invalid command.\n";
        throw InvalidMoveException();
    }
    if (from[0] == 't') {
        int tableauIndex = from[1] - '1';
        if (tableauIndex < 0 || tableauIndex >= 8 || tableau[tableauIndex].empty()) {
            cout << "Invalid Tableau index or empty Tableau column.\n";
            throw InvalidMoveException();
        }
        Card cardToMove = tableau[tableauIndex].back();
        for (size_t j = 0; j < foundations.size(); ++j) {
            if (canMoveToFoundation(cardToMove, j)) {
                foundations[j].push_back(cardToMove);
                tableau[tableauIndex].pop_back();
                cout << "Auto-moved " << cardToMove.toString() << " to Foundation " << j + 1
                     << ".\n";
                return;
            }
        }
        for (size_t j = 0; j < freecells.size(); ++j) {
            if (freecells[j].rank == 0) {
                freecells[j] = cardToMove;
                tableau[tableauIndex].pop_back();
                cout << "Auto-moved " << cardToMove.toString() << " to Freecell " << j + 1 << ".\n";
                return;
            }
        }
        cout << "No valid moves to Foundation for " << cardToMove.toString() << ".\n";
        throw InvalidMoveException();
    } else if (from[0] == 'f') {
        int freecellIndex = from[1] - '1';
        if (freecellIndex < 0 || freecellIndex >= 4 || freecells[freecellIndex].rank == 0) {
            cout << "Invalid Freecell index or empty Freecell.\n";
            throw InvalidMoveException();
        }
        Card cardToMove = freecells[freecellIndex];
        for (size_t j = 0; j < foundations.size(); ++j) {
            if (canMoveToFoundation(cardToMove, j)) {
                foundations[j].push_back(cardToMove);
                freecells[freecellIndex] = Card{' ', 0};
                cout << "Auto-moved " << cardToMove.toString() << " to Foundation " << j + 1
                     << ".\n";
                return;
            }
        }
        cout << "No valid moves to Foundation for " << cardToMove.toString() << ".\n";
        throw InvalidMoveException();
    } else {
        cout << "Invalid source. Use 'T' for Tableau or 'F' for Freecell.\n";
        throw InvalidMoveException();
    }
}
