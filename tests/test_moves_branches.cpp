#include "board.hpp"
#include "card.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("moveCard throws on empty commands and invalid source", "[moves]") {
    FreecellBoard b;
    REQUIRE_THROWS_AS(b.moveCard("", "t1"), std::exception);
    REQUIRE_THROWS_AS(b.moveCard("t1", ""), std::exception);
    REQUIRE_THROWS_AS(b.moveCard("x1", "t1"), std::exception);
}

TEST_CASE("freecell invalid index and destination handling", "[moves]") {
    FreecellBoard b;
    // empty freecell
    REQUIRE_THROWS_AS(b.moveCard("f1", "t1"), std::exception);
    // invalid freecell index
    REQUIRE_THROWS_AS(b.moveCard("f9", "t1"), std::exception);

    // place card in freecell and try invalid tableau dest
    b.freecells[0] = Card{'S', 5};
    REQUIRE_THROWS_AS(b.moveCard("f1", "t9"), std::exception);
}

TEST_CASE("tableau invalid moves and freecell occupied", "[moves]") {
    FreecellBoard b;
    // empty tableau move should return (no throw) when from tableau empty
    b.moveCard("t1", "t2", false); // should not throw

    // prepare tableau src with one card
    b.tableau[0].push_back(Card{'H', 7});
    // move to invalid dest tableau index
    REQUIRE_THROWS_AS(b.moveCard("t1", "t9"), std::exception);

    // move to freecell occupied
    b.freecells[0] = Card{'C', 2};
    REQUIRE_THROWS_AS(b.moveCard("t1", "f1"), std::exception);
}

TEST_CASE("tableau to foundation invalid move prints and doesn't throw", "[moves]") {
    FreecellBoard b;
    b.tableau[0].push_back(Card{'H', 2}); // 2H cannot go to empty foundation
    // moveCard to foundation when invalid should not throw but not change foundation
    b.moveCard("t1", "o1", false);
    REQUIRE(b.foundations[0].empty());
}
