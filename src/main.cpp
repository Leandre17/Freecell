#include "game.hpp"
#include "save.hpp"
#include <iostream>
#include <string>

using namespace std;

static void showMainMenu() {
    cout << "=== Freecell ===\n";
    cout << "1) New game\n";
    cout << "2) Load game\n";
    cout << "3) Help (commands)\n";
    cout << "4) Quit\n";
    cout << "Choose an option: ";
}

int main() {
    FreecellGame game;

    string line;
    while (true) {
        showMainMenu();
        if (!std::getline(cin, line))
            break;

        // trim leading/trailing spaces
        auto start = line.find_first_not_of(" \t\r\n");
        if (start == string::npos)
            start = 0;
        auto end = line.find_last_not_of(" \t\r\n");
        string choice = line.substr(start, end == string::npos ? string::npos : end - start + 1);

        if (choice == "1" || choice == "n" || choice == "new") {
            game.newGame();
            game.display();
            game.gameLoop();
        } else if (choice == "2" || choice == "l" || choice == "load") {
            cout << "Filename to load (default: save.txt): ";
            string fname;
            if (!std::getline(cin, fname))
                break;
            if (fname.empty())
                fname = "save.txt";
            try {
                FreecellGame loaded = SaveManager::loadGame(fname);
                game = std::move(loaded);
                cout << "Game loaded from " << fname << "\n";
                game.display();
                game.gameLoop();
            } catch (const std::exception& e) {
                cout << "Failed to load '" << fname << "': " << e.what() << "\n";
            } catch (...) {
                cout << "Failed to load '" << fname << "'\n";
            }
        } else if (choice == "3" || choice == "h" || choice == "help") {
            cout << "Menu commands:\n";
            cout << " - During a game, type 'help' to see available moves and commands.\n";
            cout << " - From this menu choose 1 to start a new game or 2 to load a saved game.\n";
            cout << "Press Enter to continue...\n";
            std::getline(cin, line);
        } else if (choice == "4" || choice == "q" || choice == "quit") {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Unknown option. Please enter 1-4.\n";
        }
    }

    return 0;
}
