#include "board.hpp"
#include "solver.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Solver hint and solve on tiny scenario") {
    FreecellBoard b;
    // Prepare a minimal scenario: foundation O1 has Ace of Hearts, tableau T1 has 2 of Hearts
    b.foundations[0].push_back(Card('H', 1));
    b.tableau[0].push_back(Card('H', 2));

    Solver s;
    auto h = s.hint(b);
    REQUIRE(!h.empty());

    auto sol = s.solve(b, 1000);
    REQUIRE(!sol.empty());
}
