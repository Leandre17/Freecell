#include "game.hpp"
using namespace std;

FreecellGame::FreecellGame() {
  board = FreecellBoard();
  newGame();
}

void FreecellGame::newGame() {
  // Clear all data structures
  for (auto &col : board.tableau)
    col.clear();
  for (auto &foundation : board.foundations)
    foundation.clear();
  for (Card &cell : board.freecells)
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

void FreecellGame::display() const {
  cout << "\n=== Freecell ===\n";

  // Display freecells
  cout << "Free Cells: ";
  for (const Card &cell : board.freecells) {
    if (cell.rank != 0) {
      cout << "[" << cell.toString() << "] ";
    } else {
      cout << "[ ] ";
    }
  }
  cout << "\n";

  // Display foundations
  cout << "Foundations: ";
  for (const auto &foundation : board.foundations) {
    if (!foundation.empty()) {
      cout << "[" << foundation.back().toString() << "] ";
    } else {
      cout << "[ ] ";
    }
  }
  cout << "\n";

  // Display tableau
  for (size_t i = 0; i < board.tableau.size(); i++) {
    cout << "T" << (i + 1) << "\t";
  }
  cout << "\n";

  size_t maxHeight = 0;
  for (const auto &col : board.tableau) {
    maxHeight = std::max(maxHeight, col.size());
  }

  for (size_t row = 0; row < maxHeight; ++row) {
    for (size_t col = 0; col < 8; ++col) {
      if (row < board.tableau[col].size()) {
        cout << board.tableau[col][row].toString() << "\t";
      } else {
        cout << "\t";
      }
    }
    cout << "\n";
  }
}

bool FreecellGame::isWon() const {
  for (const auto &foundation : board.foundations) {
    if (foundation.size() != 13)
      return false;
  }
  return true;
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
    command.erase(remove_if(command.begin(), command.end(), ::isspace),
                  command.end());

    if (command == "q" || command == "quit" || command == "exit") {
      cout << "Thanks for playing!\n";
      break;
    } else if (command == "reset") {
      newGame();
      cout << "Game has been reset.\n";
      display();
      continue;
    } else if (command == "help" || command == "?" || command == "h" ||
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
              "  auto  - Automatically move possible cards to foundations\n"
              "  undo [n] - Undo the last move or the last n moves\n";
      continue;
    } else if (command == "show" || command == "display") {
      display();
      continue;
    } else if (command == "auto") {
      board.autoMoveToFoundation();
      display();
      continue;
    } else if (command.rfind("undo", 0) == 0) {
      size_t spacePos = command.find(' ');
      if (spacePos != string::npos) {
        string nbStr = command.substr(spacePos + 1);
        try {
          int nb = stoi(nbStr);
          undoMoves(nb);
        } catch (const invalid_argument &) {
          cout << "Invalid number of moves to undo.\n";
        }
      } else {
        undoMove();
      }
      display();
      continue;
    } else {
    }
    try {
      handleMoveCommands(command);
      boardHistory.push_back(board);
      moveHistory.push_back(command);
    } catch (const std::exception &) {
    }
  }
}

void FreecellGame::handleMoveCommands(const string &command) {
  // Parse and execute the command
  size_t spacePos = command.find(' ');
  if (spacePos != string::npos) {
    string from = command.substr(0, spacePos);
    string to = command.substr(spacePos + 1);
    size_t index = to.find(' ');
    if (index == string::npos)
      board.moveCard(from, to);
    else {
      string nb = to.substr(index + 1);
      to = to.substr(0, index);
      try {
        int count = stoi(nb);
        board.moveSequence(from, to, count);
      } catch (const std::exception &) {
        cout << "Invalid number of cards to move.\n";
        throw InvalidMoveException();
      }
    }
    display();
  } else {
    board.autoMoveOneCard(command);
    display();
  }
}

void FreecellGame::undoMove() {
  if (boardHistory.empty() || boardHistory.size() <= 1) {
    cout << "No moves to undo.\n";
    return;
  }
  board = boardHistory.back();
  boardHistory.pop_back();
  cout << "Move undone.\n";
}

void FreecellGame::undoMoves(int n) {
  for (int i = 0; i < n; ++i) {
    undoMove();
  }
}