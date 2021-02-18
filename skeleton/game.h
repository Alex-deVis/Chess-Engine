#pragma once
#include <stack>
#include "board.h"

class Game {
 public:
    Game();
    ~Game();

    bool check_move(std::string move_string);
    
    void print_board();

 private:
    Board *board;
    std::stack<Configuration> *played_moves;

    bool moving_linearly(Position start, Position end);
    bool moving_diagonaly(Position start, Position end);
};