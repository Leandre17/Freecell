#include <string>

using namespace std;
class Card {
public:
  Card(char s, int r) : suit(s), rank(r) {}
  Card() : suit(' '), rank(0) {}
  Card(Card &&) = default;
  Card(const Card &) = default;
  Card &operator=(Card &&) = default;
  Card &operator=(const Card &) = default;
  ~Card() = default;

  char suit;
  int rank;
  string toString() const;
  bool isRed() const;
};