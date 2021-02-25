#pragma once
#include "game.h"

class Engine {
public:
    Engine(Game *game, Color c, int dd = 3);
    ~Engine();

    void opponent_move(std::string move_string);
    std::string generate_move();
    int do_undo_count;

private:
    Game *game;
    Game *thinking_game;
    Color color;
    int default_depth;

    double move_score(int depth, Color color);
    double evaluate_position();
};
