#include "../skeleton/engine.h"
#include <iomanip>

Engine::Engine(Game *game, Color c = Color::WHITE, int dd) {
    this->game = game;
    this->color = c;
    this->default_depth = dd;
    this->do_undo_count = 0;
}

Engine::~Engine() {
}

void Engine::opponent_move(std::string move_string) {
    this->game->move(move_string, false);
}

std::string Engine::generate_move() {
    std::pair<std::string, int> best = std::make_pair("", 0);
    if (this->color == Color::WHITE) {best.second = -INFINITY;} else {best.second = INFINITY;}
    double eval;

    int verif = 0;
    for (std::string move_string : game->possible_moves_for(color)) {
        game->move(move_string, false);
        do_undo_count++;
        verif++;
        if (this->color == Color::WHITE) {
            eval = move_score(default_depth - 1, Color::BLACK);
            if (eval > best.second) {
                best.second = eval;
                best.first = move_string;
            }
        } else {
            eval = move_score(default_depth - 1, Color::WHITE);
            if (eval < best.second) {
                best.second = eval;
                best.first = move_string;
            }
        }
        std::cout << move_string << " is rated " << std::setprecision(2) << eval << "\n";
        game->undo_move();
        do_undo_count--;
        if (verif == 10) {
            break;
        }
    }

    return best.first;
}

double Engine::move_score(int depth, Color c) {
    if (game->ended() || depth == 0) {
        return evaluate_position();
    }

    double best_eval;
    if (c == Color::WHITE) {
        best_eval = -INFINITY;
    } else {
        best_eval = INFINITY;
    }

    for (std::string move_string : game->possible_moves_for(c)) {
        game->move(move_string, false);
        do_undo_count++;
        double eval;
        if (c == Color::WHITE) {
            eval = move_score(depth - 1, Color::BLACK);
            if (eval > best_eval) {
                best_eval = eval;
            }
        } else {
            eval = move_score(depth - 1, Color::WHITE);
            if (eval < best_eval) {
                best_eval = eval;
            }
        }
        game->undo_move();
        do_undo_count--;
    }

    return best_eval;
}

double Engine::evaluate_position() {
    return game->evaluate();
}
