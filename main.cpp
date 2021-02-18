#include <iostream>
#include "./skeleton/game.h"

using namespace std;

int main() {
    Game *game = new Game();
    game->print_board();
    delete game;

    return 0;
}