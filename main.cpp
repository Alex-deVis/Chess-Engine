#include <iostream>
#include "skeleton/game.h"
#include "skeleton/engine.h"

using namespace std;

int main() {
    Game *game = new Game();
    Engine bombard(game, Color::BLACK, 4);
    string user_move;
    string engine_move;

    game->move("e2e4", false);
    game->move("b8a6", false);
    game->move("f1a6", false);
    game->move("b7a6", false);
    game->move("g1f3", false);
    game->move("d7d5", false);

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
        std::cout << "\n";
    }
    delete game;

    return 0;
}
