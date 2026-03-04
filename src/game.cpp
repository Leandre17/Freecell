#include "game.hpp"
#include "save.hpp"
using namespace std;

FreecellGame::FreecellGame() {
    board = FreecellBoard();
    newGame();
}

void FreecellGame::newGame() {
    // Clear all data structures
    for (auto& col : board.tableau)
        col.clear();
    for (auto& foundation : board.foundations)
        foundation.clear();
    for (Card& cell : board.freecells)
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
    for (const Card& cell : board.freecells) {
        if (cell.rank != 0) {
            cout << "[" << cell.toString() << "] ";
        } else {
            cout << "[ ] ";
        }
    }
    cout << "\n";

    // Display foundations
    cout << "Foundations: ";
    for (const auto& foundation : board.foundations) {
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
    for (const auto& col : board.tableau) {
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
    for (const auto& foundation : board.foundations) {
        if (foundation.size() != 13)
            return false;
    }
    return true;
}

void FreecellGame::gameLoop() {
    string command;
    display();

    while (!isWon()) {
        cout << "Enter move : ";
        getline(cin, command);
        transform(command.begin(), command.end(), command.begin(),
                  [](unsigned char c) { return std::tolower(c); });
        command.erase(command.find_last_not_of(" \n\r\t") + 1);

        if (command == "q" || command == "quit" || command == "exit") {
            cout << "Thanks for playing!\n";
            break;
        } else if (command == "reset") {
            newGame();
            cout << "Game has been reset.\n";
            display();
            continue;
        } else if (command == "help" || command == "?" || command == "h" || command.empty() ||
                   command == "commands") {
            showHelp();
            continue;
        } else if (command == "show" || command == "display") {
            display();
            continue;
        } else if (command == "auto") {
            board.autoMoveToFoundation();
            display();
            continue;
        } else if (command.rfind("save", 0) == 0) {
            // save [filename]
            string fname = "save.txt";
            size_t spacePos = command.find(' ');
            if (spacePos != string::npos) {
                string arg = command.substr(spacePos + 1);
                if (!arg.empty())
                    fname = arg;
            }
            SaveManager::saveGame(*this, fname);
            cout << "Game saved to " << fname << "\n";
            continue;
        } else if (command.rfind("load", 0) == 0) {
            // load [filename]
            string fname = "save.txt";
            size_t spacePos = command.find(' ');
            if (spacePos != string::npos) {
                string arg = command.substr(spacePos + 1);
                if (!arg.empty())
                    fname = arg;
            }
            ifstream ifs(fname);
            if (!ifs) {
                cout << "Cannot open file '" << fname << "'\n";
                continue;
            }
            FreecellGame loaded = SaveManager::loadGame(fname);
            // copy loaded state into current game
            board = loaded.board;
            boardHistory = loaded.boardHistory;
            moveHistory = loaded.moveHistory;
            cout << "Game loaded from " << fname << "\n";
            display();
            continue;
        } else if (command == "revert") {
            revert();
            continue;
        } else if (command.rfind("undo", 0) == 0) {
            size_t spacePos = command.find(' ');
            if (spacePos != string::npos) {
                string nbStr = command.substr(spacePos + 1);
                try {
                    int nb = stoi(nbStr);
                    undoMoves(nb);
                } catch (const invalid_argument&) {
                    cout << "Invalid number of moves to undo.\n";
                }
            } else {
                undoMove();
            }
            display();
            continue;
        } else {
            try {
                handleMoveCommands(command);
                boardHistory.push_back(board);
                moveHistory.push_back(command);
            } catch (const std::exception&) {
                cout << "'" << command << "' is not a valid move.\n";
                showHelp();
            }
        }
    }
    if (isWon()) {
        cout << "Congratulations! You've won the game!\n";
    }
}

void FreecellGame::handleMoveCommands(const std::string& command) {
    std::istringstream iss(command);
    std::string from, to;
    int count;

    if (iss >> from >> to) {
        if (iss >> count) {
            board.moveSequence(from, to, count);
        } else {
            board.moveCard(from, to);
        }
    } else {
        board.autoMoveOneCard(command);
    }
    display();
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
    if (n <= 0) {
        cout << "Number of moves to undo must be positive.\n";
        return;
    }
    for (int i = 0; i < n; ++i) {
        undoMove();
    }
}

void FreecellGame::revert() {
    if (boardHistory.empty()) {
        cout << "No initial state to revert to.\n";
        return;
    }
    board = boardHistory.front();
    boardHistory.clear();
    boardHistory.push_back(board);
    moveHistory.clear();
    cout << "Reverted to the initial game state.\n";
    display();
}

void FreecellGame::showHelp() const {
    cout << "Commands:\n"
            "  F1 T3 - Move card from Freecell 1 to Tableau 3\n"
            "  T2 O1 - Move card from Tableau 2 to Foundation 1\n"
            "  T4 F2 - Move card from Tableau 4 to Freecell 2\n"
            "  T5 T6 3 - Move sequence of 3 cards from Tableau 5 to Tableau 6\n"
            "  T2   - Auto-move top card from Tableau 2 to Foundation if "
            "possible\n"
            "  quit  - Quit the game\n"
            "  help  - Show this help message\n"
            "  reset - Start a new game\n"
            "  show  - Display the current game state\n"
            "  auto  - Automatically move possible cards to foundations\n"
            "  undo [n] - Undo the last move or the last n moves\n"
            "  revert - Revert to the initial game state\n"
            "Note: Freecells are labeled F1 to F4, Foundations O1 to O4, "
            "Tableau T1 to T8.\n";
}
