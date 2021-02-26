#include "../skeleton/engine.h"
#include <iomanip>
#include <sstream>

Engine::Engine(Game *game, Color c = Color::WHITE, int dd) {
    this->game = game;
    this->main_color = c;
    this->default_depth = dd;
}

Engine::~Engine() {
}

void Engine::opponent_move(std::string move_string) {
    this->game->move(move_string, false);
}

std::string Engine::generate_move() {
    std::pair<std::string, double> best = std::make_pair("", -INFINITY);
    if (main_color == Color::BLACK) best.second = INFINITY;
    Color next_color = (main_color == Color::WHITE) ? Color::BLACK : Color::WHITE;

    Game *temp_game = new Game(game);
    for (std::string move_string : temp_game->possible_moves_for(main_color)) {
        if (temp_game->move(move_string, false)) {
            double rating = rate_move(temp_game, default_depth-1, next_color);
            if ((main_color == Color::WHITE && rating > best.second) ||
                    (main_color == Color::BLACK && rating < best.second)) {
                best.first = move_string;
                best.second = rating;
            }
            temp_game->undo_move();
        }
    }
    delete temp_game;
    
    return best.first;
}

double Engine::rate_move(Game *temp_game, int depth, Color color) {
    if (temp_game->ended() || depth == 0) {
        return temp_game->evaluate();
    }
    Color next_color = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    double best_rating = (color == Color::WHITE) ? -INFINITY : INFINITY;

    for (std::string move_string : temp_game->possible_moves_for(color)) {
        temp_game->move(move_string, false);
        double rating = rate_move(temp_game, depth-1, next_color);
        if ((color == Color::WHITE && rating > best_rating) ||
                (color == Color::BLACK && rating < best_rating)) {
            best_rating = rating;
        }
        temp_game->undo_move();
    }

    return best_rating;
}
