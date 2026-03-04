#include "save.hpp"
#include <fstream>
#include <sstream>

static string cardToToken(const Card &c) {
  if (c.rank == 0)
    return "0";
  return string(1, c.suit) + to_string(c.rank);
}

static Card tokenToCard(const string &tok) {
  if (tok.empty() || tok == "0")
    return Card{' ', 0};
  char suit = tok[0];
  int rank = 0;
  try {
    rank = stoi(tok.substr(1));
  } catch (...) {
    rank = 0;
  }
  return Card{suit, rank};
}

void SaveManager::saveGame(const FreecellGame &game, const string &filename) {
  ofstream ofs(filename);
  if (!ofs)
    return;

  // Write tableau: 8 lines, each column as tokens separated by spaces
  ofs << "TABLEAU\n";
  for (size_t i = 0; i < game.board.tableau.size(); ++i) {
    const auto &col = game.board.tableau[i];
    if (col.empty()) {
      ofs << "0";
    } else {
      for (size_t j = 0; j < col.size(); ++j) {
        if (j)
          ofs << ' ';
        ofs << cardToToken(col[j]);
      }
    }
    ofs << "\n";
  }

  // Freecells: single line with 4 tokens
  ofs << "FREECELLS\n";
  for (size_t i = 0; i < game.board.freecells.size(); ++i) {
    if (i)
      ofs << ' ';
    ofs << cardToToken(game.board.freecells[i]);
  }
  ofs << "\n";

  // Foundations: 4 lines
  ofs << "FOUNDATIONS\n";
  for (size_t i = 0; i < game.board.foundations.size(); ++i) {
    const auto &f = game.board.foundations[i];
    if (f.empty()) {
      ofs << "0";
    } else {
      for (size_t j = 0; j < f.size(); ++j) {
        if (j)
          ofs << ' ';
        ofs << cardToToken(f[j]);
      }
    }
    ofs << "\n";
  }
}

FreecellGame SaveManager::loadGame(const string &filename) {
  FreecellGame game;
  ifstream ifs(filename);
  if (!ifs)
    return game;

  string line;
  // Expect TABLEAU
  if (!getline(ifs, line))
    return game;
  if (line != "TABLEAU")
    return game;

  FreecellBoard b;
  b.tableau.clear();
  b.tableau.resize(8);

  for (int i = 0; i < 8; ++i) {
    if (!getline(ifs, line))
      return game;
    istringstream iss(line);
    string tok;
    while (iss >> tok) {
      if (tok == "0")
        continue;
      b.tableau[i].push_back(tokenToCard(tok));
    }
  }

  // FREECELLS
  if (!getline(ifs, line))
    return game;
  if (line != "FREECELLS")
    return game;
  if (!getline(ifs, line))
    return game;
  {
    istringstream iss(line);
    string tok;
    b.freecells.clear();
    while (iss >> tok) {
      b.freecells.push_back(tokenToCard(tok));
    }
    // ensure size 4
    while (b.freecells.size() < 4)
      b.freecells.push_back(Card{' ', 0});
  }

  // FOUNDATIONS
  if (!getline(ifs, line))
    return game;
  if (line != "FOUNDATIONS")
    return game;
  b.foundations.clear();
  b.foundations.resize(4);
  for (int i = 0; i < 4; ++i) {
    if (!getline(ifs, line))
      return game;
    istringstream iss(line);
    string tok;
    while (iss >> tok) {
      if (tok == "0")
        continue;
      b.foundations[i].push_back(tokenToCard(tok));
    }
  }

  // Assign board and reset histories
  game.board = b;
  game.boardHistory.clear();
  game.boardHistory.push_back(b);
  game.moveHistory.clear();

  return game;
}
#include "save.hpp"
