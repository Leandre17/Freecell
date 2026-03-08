#include "board.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("initializeDeck order and size") {
    FreecellBoard b;
    std::vector<Card> deck;
    b.initializeDeck(deck);
    REQUIRE(deck.size() == 52);
    REQUIRE(deck[0] == Card('H', 1));
    REQUIRE(deck.back() == Card('S', 13));
}

TEST_CASE("dealCards distribution across tableau") {
    FreecellBoard b;
    std::vector<Card> deck;
    b.initializeDeck(deck);
    b.dealCards(deck);
    size_t total = 0;
    size_t maxsz = 0;
    size_t minsz = SIZE_MAX;
    for (const auto& col : b.tableau) {
        total += col.size();
        maxsz = std::max(maxsz, col.size());
        minsz = std::min(minsz, col.size());
    }
    REQUIRE(total == 52);
    // With 8 columns, cards per column should be either 6 or 7
    REQUIRE(maxsz <= 7);
    REQUIRE(minsz >= 6);
}

TEST_CASE("invalid indices for move checks") {
    FreecellBoard b;
    REQUIRE(!b.canMoveToFoundation(Card('H', 1), -1));
    REQUIRE(!b.canMoveToFoundation(Card('H', 1), 10));
    REQUIRE(!b.canMoveToTableau(Card('H', 1), -2));
    REQUIRE(!b.canMoveToTableau(Card('H', 1), 999));
}

TEST_CASE("moveSequence invalid alternating sequence throws") {
    FreecellBoard b;
    // same color sequence: S5 S4 should be invalid
    b.tableau[0].push_back(Card('S', 5));
    b.tableau[0].push_back(Card('S', 4));
    REQUIRE_THROWS_AS(b.moveSequence("t1", "t2", 2), InvalidMoveException);
}
