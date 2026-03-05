#include "game.hpp"
using namespace std;

void FreecellBoard::initializeDeck(vector<Card>& deck) {
    const std::vector<char> suits = {'H', 'D', 'C', 'S'};
    for (char suit : suits) {
        for (int rank = 1; rank <= 13; ++rank) {
            deck.push_back(Card(suit, rank));
        }
    }
}

void FreecellBoard::shuffleDeck(vector<Card>& deck) {
    random_device rd;
    mt19937 g(rd());
    shuffle(deck.begin(), deck.end(), g);
}

void FreecellBoard::dealCards(vector<Card>& deck) {
    size_t col = 0;
    for (const Card& card : deck) {
        tableau[col].push_back(card);
        col = (col + 1) % 8;
    }
}

FreecellBoard::FreecellBoard() {
    tableau.resize(8);
    freecells.resize(4);
    foundations.resize(4);
}
