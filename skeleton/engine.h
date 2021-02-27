#pragma once
#include "game.h"
#define INFINITY    40000

class Engine {
public:
    Engine(Game *game, Color c, int dd = 3);
    ~Engine();

    void opponent_move(std::string move_string);
    std::string generate_move();

private:
    Game *game;
    Game *thinking_game;
    Color main_color;
    int default_depth;

    int rate_move(Game *g, int depth, int alpha, int beta, Color c);

    int evaluate(Game *g);
    int evaluate_piece(Piece *p, bool endgame = false);
};
