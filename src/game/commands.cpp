#include "game.hpp"
#include "save.hpp"
#include "solver.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void FreecellGame::gameLoop() {
    std::string command;
    display();

    while (!isWon()) {
        std::cout << "Enter move : ";
        std::getline(std::cin, command);
        std::transform(command.begin(), command.end(), command.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        command.erase(command.find_last_not_of(" \n\r\t") + 1);

        if (command == "q" || command == "quit" || command == "exit") {
            std::cout << "Thanks for playing!\n";
            break;
        } else if (command == "reset") {
            newGame();
            std::cout << "Game has been reset.\n";
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
            std::string fname = "save.txt";
            size_t spacePos = command.find(' ');
            if (spacePos != std::string::npos) {
                std::string arg = command.substr(spacePos + 1);
                if (!arg.empty())
                    fname = arg;
            }
            SaveManager::saveGame(*this, fname);
            std::cout << "Game saved to " << fname << "\n";
            continue;
        } else if (command.rfind("load", 0) == 0) {
            // load [filename]
            std::string fname = "save.txt";
            size_t spacePos = command.find(' ');
            if (spacePos != std::string::npos) {
                std::string arg = command.substr(spacePos + 1);
                if (!arg.empty())
                    fname = arg;
            }
            std::ifstream ifs(fname);
            if (!ifs) {
                std::cout << "Cannot open file '" << fname << "'\n";
                continue;
            }
            FreecellGame loaded = SaveManager::loadGame(fname);
            // copy loaded state into current game
            board = loaded.board;
            boardHistory = loaded.boardHistory;
            moveHistory = loaded.moveHistory;
            std::cout << "Game loaded from " << fname << "\n";
            display();
            continue;
        } else if (command == "revert") {
            revert();
            continue;
        } else if (command == "hint") {
            Solver s;
            std::string h = s.hint(board);
            if (h.empty())
                std::cout << "No hint available.\n";
            else
                std::cout << "Hint: " << h << "\n";
            continue;
        } else if (command.rfind("solve", 0) == 0) {
            // optional: solve [maxNodes]
            size_t maxNodes = 50000;
            size_t spacePos = command.find(' ');
            if (spacePos != std::string::npos) {
                std::string arg = command.substr(spacePos + 1);
                try {
                    maxNodes = std::stoul(arg);
                } catch (...) {
                    // ignore, keep default
                }
            }
            Solver s;
            std::cout << "Solving (limit=" << maxNodes << ")...\n";
            auto path = s.solve(board, maxNodes);
            if (path.empty()) {
                std::cout << "No solution found within node limit.\n";
            } else {
                std::cout << "Solution found (" << path.size() << " moves):\n";
                for (size_t i = 0; i < path.size(); ++i)
                    std::cout << "  " << (i + 1) << ". " << path[i] << "\n";
                std::cout << "Apply solution? (y/n): ";
                std::string answer;
                std::getline(std::cin, answer);
                if (!answer.empty() && (answer[0] == 'y' || answer[0] == 'Y')) {
                    for (const auto& cmd : path) {
                        try {
                            handleMoveCommands(cmd);
                            boardHistory.push_back(board);
                            moveHistory.push_back(cmd);
                        } catch (...) {
                            std::cout << "Failed to apply move: " << cmd << "\n";
                            break;
                        }
                    }
                }
            }
            display();
            continue;
        } else if (command.rfind("undo", 0) == 0) {
            size_t spacePos = command.find(' ');
            if (spacePos != std::string::npos) {
                std::string nbStr = command.substr(spacePos + 1);
                try {
                    int nb = std::stoi(nbStr);
                    undoMoves(nb);
                } catch (const std::invalid_argument&) {
                    std::cout << "Invalid number of moves to undo.\n";
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
                std::cout << "'" << command << "' is not a valid move.\n";
                showHelp();
            }
        }
    }
    if (isWon()) {
        std::cout << "Congratulations! You've won the game!\n";
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

void FreecellGame::showHelp() const {
    std::cout << "Commands:\n"
              << "  F1 T3 - Move card from Freecell 1 to Tableau 3\n"
              << "  T2 O1 - Move card from Tableau 2 to Foundation 1\n"
              << "  T4 F2 - Move card from Tableau 4 to Freecell 2\n"
              << "  T5 T6 3 - Move sequence of 3 cards from Tableau 5 to Tableau 6\n"
              << "  T2   - Auto-move top card from Tableau 2 to Foundation if possible\n"
              << "  hint  - Show a single suggested move\n"
              << "  solve [maxNodes] - Attempt to solve (optional node limit)\n"
              << "  save [filename] - Save the current game (default: save.txt)\n"
              << "  load [filename] - Load a saved game (default: save.txt)\n"
              << "  quit  - Quit the game\n"
              << "  help  - Show this help message\n"
              << "  reset - Start a new game\n"
              << "  show  - Display the current game state\n"
              << "  auto  - Automatically move possible cards to foundations\n"
              << "  undo [n] - Undo the last move or the last n moves\n"
              << "  revert - Revert to the initial game state\n"
              << "Note: Freecells are labeled F1 to F4, Foundations O1 to O4, Tableau T1 to T8.\n";
}
