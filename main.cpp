#include <iostream>
#include "skeleton/game.h"
#include "skeleton/engine.h"

using namespace std;

void ultramove(Game *game, std::string moves) {
    for (int i=0; i<moves.size(); i+=4) {
        string m;
        m += moves[i];
        m += moves[i+1];
        m += moves[i+2];
        m += moves[i+3];
        game->move(m);
    }
}

int main() {
    Game *game = new Game();
    Engine bombard(game, Color::BLACK, 4);
    string user_move;
    string engine_move;

    ultramove(game, "d2d4b8c6b1c3e7e5g1f3e5d4e2e3d4c3d1d5g8f6d5f7e8f7f1e2f8b4");

    game->print_board();
    while(true) {
        // Player
        cin >> user_move;
        if (user_move == "exit") {
            break;
        }
        if (game->move(user_move, false)) {
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
    }
    delete game;

    return 0;
}
