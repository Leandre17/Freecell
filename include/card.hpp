#pragma once
#include <string>

using namespace std;

/**
 * @file card.hpp
 * @brief Definitions for a playing card used by the Freecell game.
 */

/**
 * @class Card
 * @brief Represents a single playing card (suit and rank).
 */
class Card {
  public:
    /**
     * Construct a card with given suit and rank.
     * @param s Suit character (e.g., 'H','D','C','S').
     * @param r Rank number (1..13).
     */
    Card(char s, int r) : suit(s), rank(r) {}
    Card() : suit(' '), rank(0) {}
    Card(Card&&) = default;
    Card(const Card&) = default;
    Card& operator=(Card&&) = default;
    Card& operator=(const Card&) = default;
    bool operator==(const Card& b) const {
        return suit == b.suit && rank == b.rank;
    }
    ~Card() = default;

    char suit;
    int rank;
    /**
     * @brief Convert the card to a short string representation (e.g., "AH").
     * @return string representation of the card.
     */
    string toString() const;

    /**
     * @brief Returns true if the card's suit is red (hearts or diamonds).
     */
    bool isRed() const;
};

/**
 * @brief Suit values used in the project.
 */
enum MyEnum { HEARTS = 'H', DIAMONDS = 'D', CLUBS = 'C', SPADES = 'S' };