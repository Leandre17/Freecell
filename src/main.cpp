#include <iostream>
#include "game.hpp"

using namespace std;


int main() {
    FreecellGame game;
    
    game.newGame();
    game.display();
    
    if (game.isWon()) {
        cout << "Congratulations! You won!\n";
    }
    
    return 0;
}
