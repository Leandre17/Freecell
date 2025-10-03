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
  cout << "\n";

  // Display foundations
  cout << "Foundations: ";
  for (const auto &foundation : foundations) {
    if (!foundation.empty()) {
      cout << "[" << foundation.back().toString() << "] ";
    } else {
      cout << "[ ] ";
    }
  }
  cout << "\n";

  // Display tableau
  for (size_t i = 0; i < tableau.size(); i++) {
    cout << "T" << (i + 1) << "  ";
  }
  cout << "\n";

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

void FreecellGame::moveCard(const string &from, const string &to) {
  cout << "Move from " << from << " to " << to
       << " (functionality not yet implemented)\n";

  if (from.empty() || to.empty()) {
    cout << "Invalid move command.\n";
    return;
  }
  if (from[0] == 'F') {
    // Moving from Freecell
    int freecellIndex = from[1] - '1';
    if (freecellIndex < 0 || freecellIndex >= 4 ||
        freecells[freecellIndex].rank == 0) {
      cout << "Invalid Freecell index or empty Freecell.\n";
      return;
    }
    Card cardToMove = freecells[freecellIndex];

    if (to[0] == 'T') {
      // Move to Tableau
      int tableauIndex = to[1] - '1';
      if (tableauIndex < 0 || tableauIndex >= 8) {
        cout << "Invalid Tableau index.\n";
        return;
      }
      if (canMoveToTableau(cardToMove, tableauIndex)) {
        tableau[tableauIndex].push_back(cardToMove);
        freecells[freecellIndex] = Card{' ', 0};
        cout << "Moved " << cardToMove.toString() << " to Tableau "
             << tableauIndex + 1 << ".\n";
      } else {
        cout << "Cannot move " << cardToMove.toString() << " to Tableau "
             << tableauIndex + 1 << ".\n";
      }
    } else if (to[0] == 'O') {
      // Move to Foundation
      int foundationIndex = to[1] - '1';
      if (foundationIndex < 0 || foundationIndex >= 4) {
        cout << "Invalid Foundation index.\n";
        return;
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
    }
  } else if (from[0] == 'T') {
    // Moving from Tableau
    int tableauIndex = from[1] - '1';
    if (tableauIndex < 0 || tableauIndex >= 8 ||
        tableau[tableauIndex].empty()) {
      cout << "Invalid Tableau index or empty Tableau column.\n";
      return;
    }
    Card cardToMove = tableau[tableauIndex].back();
    if (to[0] == 'T') {
      // Move to another Tableau
      int destTableauIndex = to[1] - '1';
      if (destTableauIndex < 0 || destTableauIndex >= 8) {
        cout << "Invalid destination Tableau index.\n";
        return;
      }
      if (canMoveToTableau(cardToMove, destTableauIndex)) {
        tableau[destTableauIndex].push_back(cardToMove);
        tableau[tableauIndex].pop_back();
        cout << "Moved " << cardToMove.toString() << " to Tableau "
             << destTableauIndex + 1 << ".\n";
      } else {
        cout << "Cannot move " << cardToMove.toString() << " to Tableau "
             << destTableauIndex + 1 << ".\n";
      }
    } else if (to[0] == 'O') {
      // Move to Foundation
      int foundationIndex = to[1] - '1';
      if (foundationIndex < 0 || foundationIndex >= 4) {
        cout << "Invalid Foundation index.\n";
        return;
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
    } else if (to[0] == 'F') {
      // Move to Freecell
      int freecellIndex = to[1] - '1';
      if (freecellIndex < 0 || freecellIndex >= 4 ||
          freecells[freecellIndex].rank != 0) {
        cout << "Invalid Freecell index or Freecell already occupied.\n";
        return;
      }
      freecells[freecellIndex] = cardToMove;
      tableau[tableauIndex].pop_back();
      cout << "Moved " << cardToMove.toString() << " to Freecell "
           << freecellIndex + 1 << ".\n";
    } else {
      cout << "Invalid destination. Use 'T' for Tableau, 'F' for Foundation, "
              "or "
              "'C' for Freecell.\n";
    }
  } else {
    cout << "Invalid source. Use 'T' for Tableau or 'F' for Freecell.\n";
  }
}

bool FreecellGame::canMoveToFoundation(const Card &card,
                                       int foundationIndex) const {
  if (foundationIndex < 0 || foundationIndex >= 4)
    return false;
  const auto &foundation = foundations[foundationIndex];
  if (foundation.empty()) {
    return card.rank == 1; // Ace can be placed on empty foundation
  }
  const Card &topCard = foundation.back();
  if (topCard.suit != card.suit)
    return false;
  if (card.rank == topCard.rank + 1)
    return true;
  return false;
}

bool FreecellGame::canMoveToTableau(const Card &card, int tableauIndex) const {
  if (tableauIndex < 0 || tableauIndex >= 8)
    return false;
  const auto &tab = tableau[tableauIndex];
  if (tab.empty()) {
    return card.rank == 13; // King can be placed on empty tableau
  }
  const Card &topCard = tab.back();
  if (topCard.isRed() == card.isRed())
    return false;
  if (card.rank == topCard.rank - 1)
    return true;
  return false;
}

int FreecellGame::howManyCanMove() const {
  int emptyFreecells = 0;
  for (const Card &cell : freecells) {
    if (cell.rank == 0)
      ++emptyFreecells;
  }
  int emptyTableaus = 0;
  for (const auto &col : tableau) {
    if (col.empty())
      ++emptyTableaus;
  }
  return (emptyFreecells + 1) * (1 + emptyTableaus);
}

void FreecellGame::autoMoveToFoundation() {
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
            cout << "Auto-moved " << cardToMove.toString() << " to Foundation "
                 << j + 1 << ".\n";
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
            cout << "Auto-moved " << cardToMove.toString() << " to Foundation "
                 << j + 1 << ".\n";
            moved = true;
            break;
          }
        }
      }
    }
  } while (moved);
}

void FreecellGame::autoMoveOneCard(const string &from) {
  if (from.empty()) {
    cout << "Invalid command.\n";
    return;
  }
  if (from[0] == 'T') {
    int tableauIndex = from[1] - '1';
    if (tableauIndex < 0 || tableauIndex >= 8 ||
        tableau[tableauIndex].empty()) {
      cout << "Invalid Tableau index or empty Tableau column.\n";
      return;
    }
    Card cardToMove = tableau[tableauIndex].back();
    for (size_t j = 0; j < foundations.size(); ++j) {
      if (canMoveToFoundation(cardToMove, j)) {
        foundations[j].push_back(cardToMove);
        tableau[tableauIndex].pop_back();
        cout << "Auto-moved " << cardToMove.toString() << " to Foundation "
             << j + 1 << ".\n";
        return;
      }
    }
    for (size_t j = 0; j < freecells.size(); ++j) {
      if (freecells[j].rank == 0) {
        freecells[j] = cardToMove;
        tableau[tableauIndex].pop_back();
        cout << "Auto-moved " << cardToMove.toString() << " to Freecell "
             << j + 1 << ".\n";
        return;
      }
    }
    cout << "No valid moves to Foundation for " << cardToMove.toString()
         << ".\n";
  } else if (from[0] == 'F') {
    int freecellIndex = from[1] - '1';
    if (freecellIndex < 0 || freecellIndex >= 4 ||
        freecells[freecellIndex].rank == 0) {
      cout << "Invalid Freecell index or empty Freecell.\n";
      return;
    }
    Card cardToMove = freecells[freecellIndex];
    for (size_t j = 0; j < foundations.size(); ++j) {
      if (canMoveToFoundation(cardToMove, j)) {
        foundations[j].push_back(cardToMove);
        freecells[freecellIndex] = Card{' ', 0};
        cout << "Auto-moved " << cardToMove.toString() << " to Foundation "
             << j + 1 << ".\n";
        return;
      }
    }
    cout << "No valid moves to Foundation for " << cardToMove.toString()
         << ".\n";
  } else {
    cout << "Invalid source. Use 'T' for Tableau or 'F' for Freecell.\n";
  }
}

void FreecellGame::gameLoop() {
  string command;
  display();

  while (true) {
    if (isWon()) {
      cout << "Congratulations! You won!\n";
      break;
    }
    cout << "Enter move : ";
    getline(cin, command);
    if (command == "q" || command == "quit" || command == "exit" ||
        command == "Q") {
      cout << "Thanks for playing!\n";
      break;
    }
    if (command == "reset") {
      newGame();
      cout << "Game has been reset.\n";
      display();
      continue;
    }
    if (command == "help" || command == "?" || command == "h" ||
        command.empty() || command == "commands" || command == "H") {
      cout << "Commands:\n"
              "  F1 T3 - Move card from Freecell 1 to Tableau 3\n"
              "  T2 O1 - Move card from Tableau 2 to Foundation 1\n"
              "  T4 F2 - Move card from Tableau 4 to Freecell 2\n"
              "  q quit- Quit the game\n"
              "  exit  - Quit the game\n"
              "  help  - Show this help message\n"
              "  reset - Start a new game\n"
              "  show  - Display the current game state\n"
              "  auto  - Automatically move possible cards to foundations\n";
      continue;
    }
    if (command == "show" || command == "display") {
      display();
      continue;
    }
    if (command == "auto") {
      autoMoveToFoundation();
      display();
      continue;
    }
    // Parse and execute the command
    size_t spacePos = command.find(' ');
    if (spacePos != string::npos) {
      string from = command.substr(0, spacePos);
      string to = command.substr(spacePos + 1);
      moveCard(from, to);
      display();
    } else {
      autoMoveOneCard(command);
      display();
    }
  }
}