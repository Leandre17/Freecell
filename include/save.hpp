
#pragma once
#include "game.hpp"
#include <fstream>

/**
 * @file save.hpp
 * @brief Persistence helpers for saving and loading game state.
 */

/**
 * @class SaveManager
 * @brief Provides static helpers to save and load `FreecellGame` instances.
 */
class SaveManager {
  public:
    /**
     * @brief Save the provided game state to a file.
     * @param game Game instance to save.
     * @param filename Path to the output file.
     */
    static void saveGame(const FreecellGame& game, const string& filename);

    /**
     * @brief Load a game instance from a file.
     * @param filename Path to the saved game file.
     * @return Loaded FreecellGame instance.
     */
    static FreecellGame loadGame(const string& filename);
};