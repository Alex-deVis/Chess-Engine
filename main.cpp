#include <iostream>
#include "./skeleton/game.h"

using namespace std;

int main() {
    Game *game = new Game();
    game->print_board();
    std::string move_string;

    while (1) {
        std::cin >> move_string;
        if (move_string == "exit") {
            break;
        }
        game->move(move_string, false);
        game->print_board();
    }
    
    delete game;

    return 0;
}