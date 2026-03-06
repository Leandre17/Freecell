#include "game.hpp"
#include <catch2/catch_test_macros.hpp>
#include <sstream>

using namespace std;

TEST_CASE("game display prints header", "[game]") {
    FreecellGame g;
    std::ostringstream oss;
    auto old = std::cout.rdbuf(oss.rdbuf());
    g.display();
    std::cout.rdbuf(old);
    REQUIRE(oss.str().find("=== Freecell ===") != std::string::npos);
}

TEST_CASE("undoMove when no history prints message", "[game]") {
    FreecellGame g;
    std::ostringstream oss;
    auto old = std::cout.rdbuf(oss.rdbuf());
    g.undoMove();
    std::cout.rdbuf(old);
    REQUIRE(oss.str().find("No moves to undo.") != std::string::npos);
}

TEST_CASE("undoMoves invalid n prints message", "[game]") {
    FreecellGame g;
    std::ostringstream oss;
    auto old = std::cout.rdbuf(oss.rdbuf());
    g.undoMoves(0);
    std::cout.rdbuf(old);
    REQUIRE(oss.str().find("must be positive") != std::string::npos);
}
