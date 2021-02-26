#include <iostream>
#include "skeleton/game.h"
#include "skeleton/engine.h"

using namespace std;

int main() {
    Game *game = new Game();
    Engine bombard(game, Color::BLACK, 4);
    string user_move;
    string engine_move;

    game->print_board();
    while(true) {
        // Player
        cin >> user_move;
        if (user_move == "exit") {
            break;
        }
        game->move(user_move, false);
        game->print_board();
        if (game->ended(false)) {
            break;
        }
        // Engine
        engine_move = bombard.generate_move();
        std::cout << "I decided to play " << engine_move << "\n";
        game->move(engine_move, false);
        game->print_board();
        if (game->ended(false)) {
            break;
        }
    }
    delete game;

    return 0;
}
