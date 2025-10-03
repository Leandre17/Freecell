#include "game.hpp"
using namespace std;

void FreecellGame::initializeDeck(vector<Card> &deck) {
  const std::vector<char> suits = {'H', 'D', 'C', 'S'};
  for (char suit : suits) {
    for (int rank = 1; rank <= 13; ++rank) {
      deck.push_back(Card(suit, rank));
    }
  }
}

void FreecellGame::shuffleDeck(vector<Card> &deck) {
  random_device rd;
  mt19937 g(rd());
  shuffle(deck.begin(), deck.end(), g);
}

void FreecellGame::dealCards(vector<Card> &deck) {
  size_t col = 0;
  for (const Card &card : deck) {
    tableau[col].push_back(card);
    col = (col + 1) % 8;
  }
}

FreecellGame::FreecellGame() {
  tableau.resize(8);
  freecells.resize(4);
  foundations.resize(4);
}

void FreecellGame::newGame() {
  // Clear all data structures
  for (auto &col : tableau)
    col.clear();
  for (auto &foundation : foundations)
    foundation.clear();
  for (Card &cell : freecells)
    cell = Card{' ', 0};

  // Create and shuffle deck
  vector<Card> deck;
  initializeDeck(deck);
  shuffleDeck(deck);
  dealCards(deck);
}

void FreecellGame::display() const {
  cout << "\n=== Freecell ===\n\n";

  // Display freecells
  cout << "Free Cells: ";
  for (const Card &cell : freecells) {
    if (cell.rank != 0) {
      cout << "[" << cell.toString() << "] ";
    } else {
      cout << "[ ] ";
    }
  }
  cout << "\n\n";

  // Display foundations
  cout << "Foundations: ";
  for (const auto &foundation : foundations) {
    if (!foundation.empty()) {
      cout << "[" << foundation.back().toString() << "] ";
    } else {
      cout << "[ ] ";
    }
  }
  cout << "\n\n";

  // Display tableau
  cout << "Tableau:\n";
  size_t maxHeight = 0;
  for (const auto &col : tableau) {
    maxHeight = std::max(maxHeight, col.size());
  }

  for (size_t row = 0; row < maxHeight; ++row) {
    for (size_t col = 0; col < 8; ++col) {
      if (row < tableau[col].size()) {
        cout << tableau[col][row].toString() << "  ";
      } else {
        cout << "    ";
      }
    }
    cout << "\n";
  }
}

bool FreecellGame::isWon() const {
  for (const auto &foundation : foundations) {
    if (foundation.size() != 13)
      return false;
  }
  return true;
}
