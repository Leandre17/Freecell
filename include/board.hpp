#pragma once
#include "card.hpp"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>
using namespace std;

/**
 * @file board.hpp
 * @brief Board representation and operations for Freecell.
 */

/**
 * @class FreecellBoard
 * @brief Stores the tableau, freecells and foundations and provides methods
 * to manipulate the board state.
 */
class FreecellBoard {
  private:
  public:
    /// 8 tableau columns
    vector<vector<Card>> tableau; // 8 columns
    /// 4 free cells available for temporary card storage
    vector<Card> freecells; // 4 free cells
    /// 4 foundation piles (one per suit)
    vector<vector<Card>> foundations; // 4 foundation piles

    /** Default constructor initializes an empty board. */
    FreecellBoard();

    /** Compare inequality between boards. */
    bool operator!=(const FreecellBoard& b) const;

    /**
     * @brief Prepare a standard 52-card deck in the provided vector.
     */
    void initializeDeck(vector<Card>& deck);

    /** Shuffle the deck. */
    void shuffleDeck(vector<Card>& deck);

    /** Deal cards from deck into the tableau columns. */
    void dealCards(vector<Card>& deck);

    /** Move a card or sequence from one location to another (string syntax).
     * @param from source location string
     * @param to destination location string
     */
    void moveCard(const string& from, const string& to, bool verbose = true);

    bool canMoveToFoundation(const Card& card, int foundationIndex) const;
    bool canMoveToTableau(const Card& card, int tableauIndex) const;
    size_t howManyCanMove() const;
    void autoMoveToFoundation();
    void autoMoveOneCard(const string& from);
    void moveSequence(const string& from, const string& to, size_t count);

    /** Display the board to stdout. */
    void display() const;
};

class InvalidMoveException : public std::exception {
  public:
    const char* what() const noexcept override {
        return "Invalid move";
    }
};