#include <iostream>
#include "game.hpp"

using namespace std;


int main() {
    FreecellGame game;
    
    game.newGame();
    game.display();
    game.gameLoop();
    
    return 0;
}
