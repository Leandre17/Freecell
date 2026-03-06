#include "game.hpp"
#include "save.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cstdio>
#include <fstream>
#include <iterator>

TEST_CASE("Save and load game roundtrip") {
    FreecellGame g;
    g.newGame();
    // make a small deterministic change
    g.undoMoves(0); // exercise API surface

    std::string fname = "test_save.tmp";
    SaveManager::saveGame(g, fname);

    FreecellGame loaded = SaveManager::loadGame(fname);
    std::string fname2 = "test_save2.tmp";
    SaveManager::saveGame(loaded, fname2);

    // compare files
    std::ifstream ifs1(fname);
    std::ifstream ifs2(fname2);
    REQUIRE(ifs1.good());
    REQUIRE(ifs2.good());
    std::string a((std::istreambuf_iterator<char>(ifs1)), std::istreambuf_iterator<char>());
    std::string b((std::istreambuf_iterator<char>(ifs2)), std::istreambuf_iterator<char>());
    REQUIRE(a == b);

    std::remove(fname.c_str());
    std::remove(fname2.c_str());
}
