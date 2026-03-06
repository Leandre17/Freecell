#include "board.hpp"
#include "solver.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Solver::hint suggests foundation moves when available", "[solver]") {
    FreecellBoard b;
    b.freecells[0] = Card{'H', 1}; // Ace hearts
    std::string h = Solver().hint(b);
    REQUIRE(!h.empty());
    REQUIRE((h.find("f1") != std::string::npos || h.find("o1") != std::string::npos ||
             h.find("o") != std::string::npos));
}

TEST_CASE("Solver::solve finds immediate foundation move", "[solver]") {
    FreecellBoard b;
    b.freecells[0] = Card{'D', 1}; // Ace diamonds
    auto path = Solver().solve(b, 1000);
    REQUIRE(!path.empty());
    REQUIRE((path[0].find("o") != std::string::npos));
}
