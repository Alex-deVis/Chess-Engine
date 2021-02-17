#include <iostream>
#include "pieces.h"
#include "move_checker.h"

using namespace std;

int main() {
    Board *game = new Board();
    game->print_current_config();
    delete game;

    return 0;
}