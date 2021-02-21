#include <iostream>
#include "./skeleton/game.h"

using namespace std;

int main() {
    Game *game = new Game();
    std::string move_string;

    // game->move("g2g4", false);
    // game->move("f1g2", false);
    // game->move("g1f3", false);
    // game->move("e1g1", false);

    // game->move("b1a3", false);
    // game->move("b2b3", false);
    // game->move("c1b2", false);
    // game->move("d2d3", false);
    // game->move("d1d2", false);
    // game->move("e1c1", false);


    while (1) {
        game->print_board();
        std::cin >> move_string;
        if (move_string == "exit") {
            break;
        }
        game->move(move_string, false);
    }
    
    delete game;

    return 0;
}