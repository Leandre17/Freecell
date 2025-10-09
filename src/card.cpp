#include "card.hpp"
using namespace std;

static bool canUseColors() {
  const char *term = getenv("TERM");
  if (!term)
    return false;
  string termStr(term);
  return termStr == "xterm" || termStr == "xterm-256color" ||
         termStr == "screen" || termStr == "screen-256color" ||
         termStr == "linux" || termStr == "cygwin";
}

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
    suitStr = canUseColors() ? "\033[1;31m♥\033[0m" : "♥";
    break;
  case DIAMONDS:
    suitStr = canUseColors() ? "\033[1;31m♦\033[0m" : "♦";
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
