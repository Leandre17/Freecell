#include "game.hpp"
using namespace std;

void FreecellBoard::moveCard(const string& from, const string& to) {
    if (from.empty() || to.empty()) {
        cout << "Invalid move command.\n";
        throw InvalidMoveException();
    }
    if (from[0] == 'f') {
        // Moving from Freecell
        int freecellIndex = from[1] - '1';
        if (freecellIndex < 0 || freecellIndex >= 4 || freecells[freecellIndex].rank == 0) {
            cout << "Invalid Freecell index or empty Freecell.\n";
            throw InvalidMoveException();
        }
        Card cardToMove = freecells[freecellIndex];

        if (to[0] == 't') {
            // Move to Tableau
            int tableauIndex = to[1] - '1';
            if (tableauIndex < 0 || tableauIndex >= 8) {
                cout << "Invalid Tableau index.\n";
                throw InvalidMoveException();
            }
            if (canMoveToTableau(cardToMove, tableauIndex)) {
                tableau[tableauIndex].push_back(cardToMove);
                freecells[freecellIndex] = Card{' ', 0};
                cout << "Moved " << cardToMove.toString() << " to Tableau " << tableauIndex + 1
                     << ".\n";
            } else {
                cout << "Cannot move " << cardToMove.toString() << " to Tableau "
                     << tableauIndex + 1 << ".\n";
            }
        } else if (to[0] == 'o') {
            // Move to Foundation
            int foundationIndex = to[1] - '1';
            if (foundationIndex < 0 || foundationIndex >= 4) {
                cout << "Invalid Foundation index.\n";
                throw InvalidMoveException();
            }
            if (canMoveToFoundation(cardToMove, foundationIndex)) {
                foundations[foundationIndex].push_back(cardToMove);
                freecells[freecellIndex] = Card{' ', 0};
                cout << "Moved " << cardToMove.toString() << " to Foundation "
                     << foundationIndex + 1 << ".\n";
            } else {
                cout << "Cannot move " << cardToMove.toString() << " to Foundation "
                     << foundationIndex + 1 << ".\n";
            }
        } else {
            cout << "Invalid destination. Use 'T' for Tableau or 'O' for "
                    "Foundation.\n";
            throw InvalidMoveException();
        }
    } else if (from[0] == 't') {
        // Moving from Tableau
        int tableauIndex = from[1] - '1';
        if (tableauIndex < 0 || tableauIndex >= 8 || tableau[tableauIndex].empty()) {
            cout << "Invalid Tableau index or empty Tableau column.\n";
            return;
        }
        Card cardToMove = tableau[tableauIndex].back();
        if (to[0] == 't') {
            // Move to another Tableau
            int destTableauIndex = to[1] - '1';
            if (destTableauIndex < 0 || destTableauIndex >= 8) {
                cout << "Invalid destination Tableau index.\n";
                throw InvalidMoveException();
            }
            if (canMoveToTableau(cardToMove, destTableauIndex)) {
                tableau[destTableauIndex].push_back(cardToMove);
                tableau[tableauIndex].pop_back();
                cout << "Moved " << cardToMove.toString() << " to Tableau " << destTableauIndex + 1
                     << ".\n";
            } else {
                cout << "Cannot move " << cardToMove.toString() << " to Tableau "
                     << destTableauIndex + 1 << ".\n";
            }
        } else if (to[0] == 'o') {
            // Move to Foundation
            int foundationIndex = to[1] - '1';
            if (foundationIndex < 0 || foundationIndex >= 4) {
                cout << "Invalid Foundation index.\n";
                throw InvalidMoveException();
            }
            if (canMoveToFoundation(cardToMove, foundationIndex)) {
                foundations[foundationIndex].push_back(cardToMove);
                tableau[tableauIndex].pop_back();
                cout << "Moved " << cardToMove.toString() << " to Foundation "
                     << foundationIndex + 1 << ".\n";
            } else {
                cout << "Cannot move " << cardToMove.toString() << " to Foundation "
                     << foundationIndex + 1 << ".\n";
            }
        } else if (to[0] == 'f') {
            // Move to Freecell
            int freecellIndex = to[1] - '1';
            if (freecellIndex < 0 || freecellIndex >= 4 || freecells[freecellIndex].rank != 0) {
                cout << "Invalid Freecell index or Freecell already occupied.\n";
                throw InvalidMoveException();
            }
            freecells[freecellIndex] = cardToMove;
            tableau[tableauIndex].pop_back();
            cout << "Moved " << cardToMove.toString() << " to Freecell " << freecellIndex + 1
                 << ".\n";
        } else {
            cout << "Invalid destination. Use 'T' for Tableau, 'F' for Foundation, "
                    "or "
                    "'C' for Freecell.\n";
            throw InvalidMoveException();
        }
    } else {
        cout << "Invalid source. Use 'T' for Tableau or 'F' for Freecell.\n";
        throw InvalidMoveException();
    }
}

void FreecellBoard::moveSequence(const string& from, const string& to, size_t count) {
    if (from.empty() || to.empty() || count <= 0 || from[0] != 't' || to[0] != 't') {
        cout << "Invalid move command.\n";
        throw InvalidMoveException();
    } else if (count > howManyCanMove()) {
        cout << "Cannot move more than " << howManyCanMove() << " cards at once.\n";
        throw InvalidMoveException();
    }
    int tableauIndex = from[1] - '1';
    int tableauDestination = to[1] - '1';
    if (tableauIndex < 0 || tableauIndex >= 8 || tableau[tableauIndex].empty()) {
        cout << "Invalid Tableau index or empty Tableau column.\n";
        throw InvalidMoveException();
    }
    if (tableauDestination < 0 || tableauDestination >= 8) {
        cout << "Invalid destination Tableau index.\n";
        throw InvalidMoveException();
    }
    if (tableau[tableauIndex].size() < count) {
        cout << "Not enough cards in the source Tableau column.\n";
        throw InvalidMoveException();
    }
    // Check if the sequence is valid
    for (size_t i = 0; i < count - 1; ++i) {
        const Card& currentCard = tableau[tableauIndex][tableau[tableauIndex].size() - count + i];
        const Card& nextCard = tableau[tableauIndex][tableau[tableauIndex].size() - count + i + 1];
        if (currentCard.rank != nextCard.rank + 1 || currentCard.isRed() == nextCard.isRed()) {
            cout << "Invalid card sequence.\n";
            throw InvalidMoveException();
        }
    }
    // Check if the move to destination tableau is valid
    const Card& movingCard = tableau[tableauIndex][tableau[tableauIndex].size() - count];
    if (!canMoveToTableau(movingCard, tableauDestination)) {
        cout << "Cannot move sequence to the destination Tableau.\n";
        throw InvalidMoveException();
    }
    // Move the cards
    for (size_t i = 0; i < count; ++i) {
        Card cardToMove = tableau[tableauIndex][tableau[tableauIndex].size() - count + i];
        tableau[tableauDestination].push_back(cardToMove);
    }
    tableau[tableauIndex].resize(tableau[tableauIndex].size() - count);
    cout << "Moved sequence of " << count << " cards from Tableau " << tableauIndex + 1
         << " to Tableau " << tableauDestination + 1 << ".\n";
}
