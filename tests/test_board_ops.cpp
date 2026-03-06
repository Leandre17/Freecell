#include "board.hpp"
#include "card.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("move tableau to freecell and back to tableau and foundation", "[board]") {
    FreecellBoard b;
    // ensure initial sizes
    REQUIRE(b.tableau.size() == 8);
    REQUIRE(b.freecells.size() == 4);
    REQUIRE(b.foundations.size() == 4);

    // tableau -> freecell
    b.tableau[0].push_back(Card{'H', 1}); // Ace of Hearts
    b.moveCard("t1", "f1", false);
    REQUIRE(b.freecells[0] == Card{'H', 1});
    REQUIRE(b.tableau[0].empty());

    // freecell -> foundation
    b.moveCard("f1", "o1", false);
    REQUIRE(b.freecells[0] == Card{' ', 0});
    REQUIRE(!b.foundations[0].empty());
    REQUIRE(b.foundations[0].back() == Card{'H', 1});
}

TEST_CASE("moveSequence valid and invalid cases", "[board]") {
    FreecellBoard b;
    // prepare source tableau with sequence 5H,4C,3H
    b.tableau[0].push_back(Card{'H', 5});
    b.tableau[0].push_back(Card{'C', 4});
    b.tableau[0].push_back(Card{'H', 3});
    // destination top is 6C so 5H can be placed
    b.tableau[1].push_back(Card{'C', 6});

    b.moveSequence("t1", "t2", 3);
    REQUIRE(b.tableau[0].empty());
    REQUIRE(b.tableau[1].size() >= 3);
    REQUIRE(b.tableau[1].back() == Card{'H', 3});

    // invalid sequence (not alternating)
    b.tableau[2].push_back(Card{'H', 7});
    b.tableau[2].push_back(Card{'H', 6}); // same color -> invalid
    REQUIRE_THROWS_AS(b.moveSequence("t3", "t4", 2), std::exception);
}

TEST_CASE("autoMoveToFoundation moves Aces from tableau and freecells", "[board]") {
    FreecellBoard b;
    b.tableau[0].push_back(Card{'D', 1}); // Ace diamonds
    b.freecells[0] = Card{'C', 1};        // Ace clubs

    b.autoMoveToFoundation();

    // foundations should contain the two aces
    bool foundD = false, foundC = false;
    for (const auto& f : b.foundations) {
        for (const auto& c : f) {
            if (c == Card{'D', 1})
                foundD = true;
            if (c == Card{'C', 1})
                foundC = true;
        }
    }
    REQUIRE(foundD);
    REQUIRE(foundC);
}

TEST_CASE("autoMoveOneCard throws on invalid source", "[board]") {
    FreecellBoard b;
    REQUIRE_THROWS_AS(b.autoMoveOneCard("x1"), std::exception);
}
