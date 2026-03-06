#include "game.hpp"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <sstream>

using namespace std;

TEST_CASE("gameLoop handles hint, save and load", "[game][loop]") {
    FreecellGame g;
    namespace fs = std::filesystem;
    fs::path tmp = fs::temp_directory_path() / "freecell_test_save.txt";
    if (fs::exists(tmp))
        fs::remove(tmp);

    string input =
        string("hint\nsave ") + tmp.string() + string("\nload ") + tmp.string() + string("\nq\n");
    std::istringstream iss(input);
    auto oldcin = std::cin.rdbuf(iss.rdbuf());
    std::ostringstream oss;
    auto oldcout = std::cout.rdbuf(oss.rdbuf());

    g.gameLoop();

    std::cout.rdbuf(oldcout);
    std::cin.rdbuf(oldcin);

    string out = oss.str();
    REQUIRE(
        ((out.find("No hint available.") != string::npos) || (out.find("Hint:") != string::npos)));
    REQUIRE(out.find("Game saved to") != string::npos);
    REQUIRE(out.find("Game loaded from") != string::npos);

    if (fs::exists(tmp))
        fs::remove(tmp);
}
