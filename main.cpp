#include <iostream>
#include "skeleton/game.h"

using namespace std;

int main() {
    Game *game = new Game();
    string user_move;
    
    // game->move("b1b3");
    // game->move("c1c3");
    // game->move("d1d3");
    // game->move("f1f3");
    // game->move("g1g3");

    game->print_board();
    while(true) {
        cin >> user_move;
        if (user_move == "undo") {
            game->undo_move();
        } else if (user_move == "exit") {
            break;
        } else {
            game->move(user_move, false);
        }
        game->print_board();
    }

    return 0;
}