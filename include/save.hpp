
#pragma once
#include "game.hpp"
#include <fstream>

class SaveManager {
public:
  static void saveGame(const FreecellGame &game, const string &filename);
  static FreecellGame loadGame(const string &filename);
};