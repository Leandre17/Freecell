#include "solver.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("solver finds immediate foundation move") {
    FreecellBoard b;
    b.tableau[0].push_back(Card('H', 1));
    Solver s;
    auto path = s.solve(b, 1000);
    REQUIRE(!path.empty());
    // Expect the first move to place the Ace to a foundation (t1 o1)
    REQUIRE(path[0].find("t1") != std::string::npos);
    REQUIRE(path[0].find("o") != std::string::npos);
}
