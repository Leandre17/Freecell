#include "game.hpp"
#include "save.hpp"
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <sstream>

TEST_CASE("history undo and revert via gameLoop") {
    // Create a deterministic saved game where T1 top card is A♥
    const std::string fname = "test_history_save.txt";
    std::ofstream ofs(fname);
    REQUIRE(ofs);
    // TABLEAU: 8 lines
    ofs << "TABLEAU\n";
    // Column 1: single Ace of Hearts
    ofs << "H1\n";
    // Columns 2..8 empty
    for (int i = 0; i < 7; ++i)
        ofs << "0\n";
    // FREECELLS
    ofs << "FREECELLS\n";
    ofs << "0 0 0 0\n";
    // FOUNDATIONS
    ofs << "FOUNDATIONS\n";
    for (int i = 0; i < 4; ++i)
        ofs << "0\n";
    ofs.close();

    // Load deterministic game
    FreecellGame g = SaveManager::loadGame(fname);

    // Prepare input: move T1 to O1, undo, revert, then quit
    std::istringstream input("t1 o1\nundo\nrevert\nquit\n");
    std::ostringstream output;
    auto oldcin = std::cin.rdbuf(input.rdbuf());
    auto oldcout = std::cout.rdbuf(output.rdbuf());

    g.gameLoop();

    std::cin.rdbuf(oldcin);
    std::cout.rdbuf(oldcout);

    std::string out = output.str();
    // Expect move to foundation, undo message, and revert message
    REQUIRE(out.find("Moved") != std::string::npos);
    REQUIRE(out.find("Foundation") != std::string::npos);
    REQUIRE((out.find("Move undone") != std::string::npos ||
             out.find("No moves to undo.") != std::string::npos));
    REQUIRE(out.find("Reverted to the initial game state.") != std::string::npos);
}
