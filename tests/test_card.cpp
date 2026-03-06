#include "card.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Card basic properties") {
    Card a('H', 1);
    REQUIRE(a.isRed() == true);
    std::string s = a.toString();
    REQUIRE(!s.empty());
    REQUIRE(s.find("A") != std::string::npos);

    Card k('S', 13);
    REQUIRE(k.isRed() == false);
    REQUIRE(k.toString().find("K") != std::string::npos);
}
