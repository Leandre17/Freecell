#include "game.hpp"
#include <iostream>

using namespace std;

int main() {
    FreecellGame game;

    game.newGame();
    game.display();
    game.gameLoop();

    return 0;
}
