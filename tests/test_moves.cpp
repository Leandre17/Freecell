#include "board.hpp"
#include "game.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("howManyCanMove default calculation") {
    FreecellBoard b;
    // default: 4 empty freecells, 8 empty tableaus => (4+1)*(1+8)=45
    REQUIRE(b.howManyCanMove() == 45);
}

TEST_CASE("moveCard tableau -> freecell and freecell -> foundation") {
    FreecellBoard b;
    b.tableau[0].push_back(Card('H', 2));

    b.moveCard("t1", "f1");
    REQUIRE(b.freecells[0].rank == 2);
    REQUIRE(b.tableau[0].empty());

    // place an Ace into freecell and move to foundation
    b.freecells[0] = Card('H', 1);
    b.moveCard("f1", "o1");
    REQUIRE(b.foundations[0].size() == 1);
    REQUIRE(b.freecells[0].rank == 0);
}

TEST_CASE("moveCard invalid destination throws") {
    FreecellBoard b;
    b.tableau[0].push_back(Card('H', 2));
    REQUIRE_THROWS_AS(b.moveCard("t1", "x1"), InvalidMoveException);
}

TEST_CASE("moveSequence valid and invalid counts") {
    FreecellBoard b;
    // build valid descending alternating sequence: S5 H4 S3
    b.tableau[0].push_back(Card('S', 5));
    b.tableau[0].push_back(Card('H', 4));
    b.tableau[0].push_back(Card('S', 3));

    size_t before = b.tableau[1].size();
    b.moveSequence("t1", "t2", 3);
    REQUIRE(b.tableau[1].size() == before + 3);

    // moving too many should throw
    REQUIRE_THROWS_AS(b.moveSequence("t2", "t3", b.howManyCanMove() + 1), InvalidMoveException);
}

TEST_CASE("autoMoveToFoundation and autoMoveOneCard behaviors") {
    FreecellBoard b;
    // autoMoveToFoundation should move Ace from tableau to foundation
    b.tableau[0].push_back(Card('D', 1));
    b.autoMoveToFoundation();
    size_t fcount = 0;
    for (const auto& f : b.foundations)
        fcount += f.size();
    REQUIRE(fcount == 1);

    // autoMoveOneCard from freecell when possible
    FreecellBoard bb;
    bb.freecells[0] = Card('C', 1);
    bb.autoMoveOneCard("f1");
    bool moved = false;
    for (const auto& f : bb.foundations)
        if (!f.empty())
            moved = true;
    REQUIRE(moved);
}
