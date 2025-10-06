#include "card.hpp"
using namespace std;

string Card::toString() const {
  string rankStr;
  string suitStr;

  if (rank == 1)
    rankStr = "A";
  else if (rank == 11)
    rankStr = "J";
  else if (rank == 12)
    rankStr = "Q";
  else if (rank == 13)
    rankStr = "K";
  else
    rankStr = to_string(rank);
  switch (suit) {
  case HEARTS:
    suitStr = "\033[1;31m♥\033[0m";
    break;
  case DIAMONDS:
    suitStr = "\033[1;31m♦\033[0m";
    break;
  case CLUBS:
    suitStr = "♣";
    break;
  case SPADES:
    suitStr = "♠";
    break;
  default:
    break;
  }
  return rankStr + suitStr;
}

bool Card::isRed() const { return suit == HEARTS || suit == DIAMONDS; }
