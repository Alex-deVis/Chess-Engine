#pragma once
#include "game.h"

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

    double rate_move(Game *g, int depth, Color c);
};
