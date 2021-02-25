#include <iostream>
#include "skeleton/game.h"
#include "skeleton/engine.h"

using namespace std;

int main() {
    Game *game = new Game();
    Engine bombard(game, Color::WHITE, 3);
    string user_move;
    string engine_move;

    game->move("g7g5");
    game->move("f7f5");
    game->move("d1h6");

    game->print_board();
    while(true) {
        engine_move = bombard.generate_move();
        std::cout << "I decided to play " << engine_move << "\n";
        game->move(engine_move, false);
        game->print_board();
        cin >> user_move;
        if (user_move == "exit") {
            std::cout << bombard.do_undo_count << "\n";
            break;
        }
        game->move(user_move, false);
        game->print_board();
    }
    delete game;

    return 0;
}