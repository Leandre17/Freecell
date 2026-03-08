#include "card.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Card default and operators") {
    Card d;
    REQUIRE(d.rank == 0);
    REQUIRE(d.suit == ' ');

    Card a('H', 1);
    Card a2('H', 1);
    REQUIRE(a == a2);

    Card ten('S', 10);
    std::string s = ten.toString();
    REQUIRE(s.find("10") != std::string::npos);
}
