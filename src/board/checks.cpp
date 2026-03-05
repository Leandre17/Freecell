#include "game.hpp"
using namespace std;

bool FreecellBoard::canMoveToFoundation(const Card& card, int foundationIndex) const {
    if (foundationIndex < 0 || foundationIndex >= 4)
        return false;
    const auto& foundation = foundations[foundationIndex];
    if (foundation.empty()) {
        return card.rank == 1; // Ace can be placed on empty foundation
    }
    const Card& topCard = foundation.back();
    if (topCard.suit != card.suit)
        return false;
    if (card.rank == topCard.rank + 1)
        return true;
    return false;
}

bool FreecellBoard::canMoveToTableau(const Card& card, int tableauIndex) const {
    if (tableauIndex < 0 || tableauIndex >= 8)
        return false;
    const auto& tab = tableau[tableauIndex];
    if (tab.empty()) {
        return true; // Any card can be placed on empty tableau
    }
    const Card& topCard = tab.back();
    if (topCard.isRed() == card.isRed())
        return false;
    if (card.rank == topCard.rank - 1)
        return true;
    return false;
}
