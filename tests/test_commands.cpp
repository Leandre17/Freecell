#include "game.hpp"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;

static string runGameWithInput(const string& input) {
    FreecellGame g;
    std::istringstream iss(input);
    auto oldin = std::cin.rdbuf(iss.rdbuf());
    std::ostringstream oss;
    auto oldout = std::cout.rdbuf(oss.rdbuf());
    g.gameLoop();
    std::cout.rdbuf(oldout);
    std::cin.rdbuf(oldin);
    return oss.str();
}

TEST_CASE("save and load via commands", "[commands]") {
    const string fname = "test_cmd_save.txt";
    if (std::filesystem::exists(fname))
        std::filesystem::remove(fname);

    string input = string("save ") + fname + "\nload " + fname + "\nquit\n";
    string out = runGameWithInput(input);

    REQUIRE(out.find("Game saved to " + fname) != string::npos);
    REQUIRE(out.find("Game loaded from " + fname) != string::npos);

    if (std::filesystem::exists(fname))
        std::filesystem::remove(fname);
}

TEST_CASE("load nonexistent prints cannot open", "[commands]") {
    string fname = "definitely_missing_file_12345.txt";
    string out = runGameWithInput("load " + fname + "\nquit\n");
    REQUIRE(out.find("Cannot open file '" + fname + "'") != string::npos);
}

// TEST_CASE("hint prints no hint", "[commands]") {
//     string out = runGameWithInput("hint\nquit\n");
//     REQUIRE(out.find("No hint available.") != string::npos);
// }

TEST_CASE("solve with zero limit prints no solution", "[commands]") {
    string out = runGameWithInput("solve 0\nquit\n");
    REQUIRE(out.find("No solution found within node limit.") != string::npos);
}

TEST_CASE("undo invalid number prints message", "[commands]") {
    string out = runGameWithInput("undo notanumber\nquit\n");
    REQUIRE(out.find("Invalid number of moves to undo.") != string::npos);
}
