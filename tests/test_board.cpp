#include "board.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Deck initialization and dealing") {
    FreecellBoard b;
    std::vector<Card> deck;
    b.initializeDeck(deck);
    REQUIRE(deck.size() == 52);

    b.dealCards(deck);
    size_t total = 0;
    for (const auto& col : b.tableau)
        total += col.size();
    REQUIRE(total == 52);
}

TEST_CASE("Can move rules to foundation and tableau") {
    FreecellBoard b;
    // empty foundation should accept Ace
    Card ace('H', 1);
    REQUIRE(b.canMoveToFoundation(ace, 0));

    // tableau rules: place red on black and rank-1
    b.tableau[0].push_back(Card('S', 5));
    REQUIRE(b.canMoveToTableau(Card('H', 4), 0));
    REQUIRE(!b.canMoveToTableau(Card('D', 6), 0));
}
