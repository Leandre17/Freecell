#pragma once
#include "board.hpp"
#include <string>
#include <vector>

/**
 * @file solver.hpp
 * @brief Simple solver helpers: a hint generator and a full (bounded) solver.
 */

class Solver {
  public:
    Solver() = default;

    /**
     * @brief Produce a single move hint for the given board. Returns empty
     * string if no move found.
     */
    std::string hint(const FreecellBoard& board) const;

    /**
     * @brief Attempt to solve the given board. Returns the list of commands
     * (as strings) leading to a solution, or an empty vector if no solution
     * was found within the given node limit.
     *
     * The search is a BFS with a node visit limit to avoid excessive runtime.
     */
    std::vector<std::string> solve(const FreecellBoard& board, size_t maxNodes = 50000) const;
};
