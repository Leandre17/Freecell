#include "card.hpp"
using namespace std;

string Card::toString() const {
  string rankStr;
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
  return rankStr + suit;
}

bool Card::isRed() const { return suit == 'H' || suit == 'D'; }
