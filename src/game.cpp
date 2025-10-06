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
  cout << "\n=== Freecell ===\n";

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
  if (from.empty() || to.empty()) {
    cout << "Invalid move command.\n";
    return;
  }
  if (from[0] == 'f') {
    // Moving from Freecell
    int freecellIndex = from[1] - '1';
    if (freecellIndex < 0 || freecellIndex >= 4 ||
        freecells[freecellIndex].rank == 0) {
      cout << "Invalid Freecell index or empty Freecell.\n";
      return;
    }
    Card cardToMove = freecells[freecellIndex];

    if (to[0] == 't') {
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
    } else if (to[0] == 'o') {
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
  } else if (from[0] == 't') {
    // Moving from Tableau
    int tableauIndex = from[1] - '1';
    if (tableauIndex < 0 || tableauIndex >= 8 ||
        tableau[tableauIndex].empty()) {
      cout << "Invalid Tableau index or empty Tableau column.\n";
      return;
    }
    Card cardToMove = tableau[tableauIndex].back();
    if (to[0] == 't') {
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
    } else if (to[0] == 'o') {
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
    } else if (to[0] == 'f') {
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
    return true; // Any card can be placed on empty tableau
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
  if (from[0] == 't') {
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
  } else if (from[0] == 'f') {
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

void FreecellGame::moveSequence(const string &from, const string &to,
                                int count) {
  if (from.empty() || to.empty() || count <= 0 || from[0] != 't' ||
      to[0] != 't') {
    cout << "Invalid move command.\n";
    return;
  } else if (count > howManyCanMove()) {
    cout << "Cannot move more than " << howManyCanMove() << " cards at once.\n";
    return;
  }
  int tableauIndex = from[1] - '1';
  int tableauDestination = to[1] - '1';
  if (tableauIndex < 0 || tableauIndex >= 8 || tableau[tableauIndex].empty()) {
    cout << "Invalid Tableau index or empty Tableau column.\n";
    return;
  }
  if (tableauDestination < 0 || tableauDestination >= 8) {
    cout << "Invalid destination Tableau index.\n";
    return;
  }
  if (tableau[tableauIndex].size() < count) {
    cout << "Not enough cards in the source Tableau column.\n";
    return;
  }
  // Check if the sequence is valid
  for (int i = 0; i < count - 1; ++i) {
    const Card &currentCard =
        tableau[tableauIndex][tableau[tableauIndex].size() - count + i];
    const Card &nextCard =
        tableau[tableauIndex][tableau[tableauIndex].size() - count + i + 1];
    if (currentCard.rank != nextCard.rank + 1 ||
        currentCard.isRed() == nextCard.isRed()) {
      cout << "Invalid card sequence.\n";
      std::cout << "Current: " << currentCard.toString()
                << ", Next: " << nextCard.toString() << "\n"; // Debug info
      return;
    }
  }
  // Check if the move to destination tableau is valid
  const Card &movingCard =
      tableau[tableauIndex][tableau[tableauIndex].size() - count];
  if (!canMoveToTableau(movingCard, tableauDestination)) {
    cout << "Cannot move sequence to the destination Tableau.\n";
    return;
  }
  // Move the cards
  for (int i = 0; i < count; ++i) {
    Card cardToMove =
        tableau[tableauIndex][tableau[tableauIndex].size() - count + i];
    tableau[tableauDestination].push_back(cardToMove);
  }
  tableau[tableauIndex].resize(tableau[tableauIndex].size() - count);
  cout << "Moved sequence of " << count << " cards from Tableau "
       << tableauIndex + 1 << " to Tableau " << tableauDestination + 1 << ".\n";
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
    transform(command.begin(), command.end(), command.begin(),
              [](unsigned char c) { return std::tolower(c); });

    if (command == "q" || command == "quit" || command == "exit") {
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
        command.empty() || command == "commands") {
      cout << "Commands:\n"
              "  F1 T3 - Move card from Freecell 1 to Tableau 3\n"
              "  T2 O1 - Move card from Tableau 2 to Foundation 1\n"
              "  T4 F2 - Move card from Tableau 4 to Freecell 2\n"
              "  T5 T6 3 - Move sequence of 3 cards from Tableau 5 to "
              "Tableau 6\n"
              "  T2   - Auto-move top card from Tableau 2 to Foundation if "
              "possible\n"
              "  quit  - Quit the game\n"
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
      size_t index = to.find(' ');
      if (index == string::npos)
        moveCard(from, to);
      else {
        string nb = to.substr(index + 1);
        to = to.substr(0, index);
        moveSequence(from, to, stoi(nb));
      }
      display();
    } else {
      autoMoveOneCard(command);
      display();
    }
  }
}