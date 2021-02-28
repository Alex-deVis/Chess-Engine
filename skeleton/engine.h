#pragma once
#include "game.h"
#define INFINITY    40000

class Engine {
public:
    Engine(int dd = 3, Color c = Color::WHITE);
    ~Engine();

    void play_move(std::string move_string, bool forced = false);
    std::string generate_move();
    void set_color(Color color);
    void new_game();
    Game *game;
private:
    Color main_color;
    int default_depth;

    int rate_move(Game *g, int depth, int alpha, int beta, Color c);

    int evaluate(Game *g);
    int evaluate_piece(Piece *p, bool endgame = false);
};
