#include "../skeleton/engine.h"
#include <iomanip>
#include <sstream>

const int pawns_points[8][8] =  {{0,  0,  0,  0,  0,  0,  0,  0},
                                {50, 50, 50, 50, 50, 50, 50, 50},
                                {10, 10, 20, 30, 30, 20, 10, 10},
                                {5,  5, 10, 25, 25, 10,  5,  5},
                                {0,  0,  0, 20, 20,  0,  0,  0},
                                {5, -5,-10,  0,  0,-10, -5,  5},
                                {5, 10, 10,-20,-20, 10, 10,  5},
                                {0,  0,  0,  0,  0,  0,  0,  0}};

const int knight_points[8][8] = {{-50,-40,-30,-30,-30,-30,-40,-50},
                                {-40,-20,  0,  0,  0,  0,-20,-40},
                                {-30,  0, 10, 15, 15, 10,  0,-30},
                                {-30,  5, 15, 20, 20, 15,  5,-30},
                                {-30,  0, 15, 20, 20, 15,  0,-30},
                                {-30,  5, 10, 15, 15, 10,  5,-30},
                                {-40,-20,  0,  5,  5,  0,-20,-40},
                                {-50,-40,-30,-30,-30,-30,-40,-50}};

const int bishop_points[8][8] = {{-20,-10,-10,-10,-10,-10,-10,-20},
                                {-10,  0,  0,  0,  0,  0,  0,-10},
                                {-10,  0,  5, 10, 10,  5,  0,-10},
                                {-10,  5,  5, 10, 10,  5,  5,-10},
                                {-10,  0, 10, 10, 10, 10,  0,-10},
                                {-10, 10, 10, 10, 10, 10, 10,-10},
                                {-10,  5,  0,  0,  0,  0,  5,-10},
                                {-20,-10,-10,-10,-10,-10,-10,-20}};

const int rook_points[8][8] =   {{0,  0,  0,  0,  0,  0,  0,  0},
                                {5, 10, 10, 10, 10, 10, 10,  5},
                                {-5,  0,  0,  0,  0,  0,  0, -5},
                                {-5,  0,  0,  0,  0,  0,  0, -5},
                                {-5,  0,  0,  0,  0,  0,  0, -5},
                                {-5,  0,  0,  0,  0,  0,  0, -5},
                                {-5,  0,  0,  0,  0,  0,  0, -5},
                                {0,  0,  0,  5,  5,  0,  0,  0}};

const int queen_points[8][8] =  {{-20,-10,-10, -5, -5,-10,-10,-20},
                                {-10,  0,  0,  0,  0,  0,  0,-10},
                                {-10,  0,  5,  5,  5,  5,  0,-10},
                                {-5,  0,  5,  5,  5,  5,  0, -5},
                                {0,  0,  5,  5,  5,  5,  0, -5},
                                {-10,  5,  5,  5,  5,  5,  0,-10},
                                {-10,  0,  5,  0,  0,  0,  0,-10},
                                {-20,-10,-10, -5, -5,-10,-10,-20}};

const int king_mid_points[8][8] =   {{-30,-40,-40,-50,-50,-40,-40,-30},
                                    {-30,-40,-40,-50,-50,-40,-40,-30},
                                    {-30,-40,-40,-50,-50,-40,-40,-30},
                                    {-30,-40,-40,-50,-50,-40,-40,-30},
                                    {-20,-30,-30,-40,-40,-30,-30,-20},
                                    {-10,-20,-20,-20,-20,-20,-20,-10},
                                    {20, 20,  0,  0,  0,  0, 20, 20},
                                    {20, 30, 10,  0,  0, 10, 30, 20}};

const int king_end_points[8][8] =   {{-50,-40,-30,-20,-20,-30,-40,-50},
                                    {-30,-20,-10,  0,  0,-10,-20,-30},
                                    {-30,-10, 20, 30, 30, 20,-10,-30},
                                    {-30,-10, 30, 40, 40, 30,-10,-30},
                                    {-30,-10, 30, 40, 40, 30,-10,-30},
                                    {-30,-10, 20, 30, 30, 20,-10,-30},
                                    {-30,-30,  0,  0,  0,  0,-30,-30},
                                    {-50,-30,-30,-30,-30,-30,-30,-50}};

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

int Engine::evaluate(Game *g) {
    if (g->ended()) {
        return g->who_won() * INFINITY;
    }
    Board *b=g->get_board();
    Piece *p;
    int score=0;
    int ct=g->get_taken(Color::WHITE) + g->get_taken(Color::BLACK);
    for (int r=1; r<=8; r++) {
        for (char c='a'; c<='h'; c++) {
            p = b->get_piece_at(Position(c, r));
            if (p) {
                if (p->type == Type::K) {
                    if (p->color == Color::WHITE) {
                        score += evaluate_piece(p, g->in_the_endgame());
                    } else {
                        score -= evaluate_piece(p, g->in_the_endgame());
                    }
                } else {
                    if (p->color == Color::WHITE) {
                        score += evaluate_piece(p);
                    } else {
                        score -= evaluate_piece(p);
                    }
                }
                ct++;
                if (ct==32) {
                    break;
                }
            }
        }
    }
    return score;
}

int Engine::evaluate_piece(Piece *p, bool endgame) {
    switch (p->type) {
        case Type::K:
            if (p->color == Color::WHITE) {
                if (!endgame) {
                    return p->points + king_mid_points[8-p->pos.row][p->pos.col-'a'];
                } else {
                    return p->points + king_end_points[8-p->pos.row][p->pos.col-'a'];
                }
            }
            if (!endgame) {
                return p->points + king_mid_points[8-p->pos.row][p->pos.col-'a'];
            } else {
                return p->points + king_end_points[8-p->pos.row][p->pos.col-'a'];
            }
        case Type::Q:
            if (p->color == Color::WHITE) {
                return p->points + queen_points[8-p->pos.row][p->pos.col-'a'];
            }
            return p->points + queen_points[p->pos.row-1][p->pos.col-'a'];
        case Type::R:
            if (p->color == Color::WHITE) {
                return p->points + rook_points[8-p->pos.row][p->pos.col-'a'];
            }
            return p->points + rook_points[p->pos.row-1][p->pos.col-'a'];
        case Type::B:
            if (p->color == Color::WHITE) {
                return p->points + bishop_points[8-p->pos.row][p->pos.col-'a'];
            }
            return p->points + bishop_points[p->pos.row-1][p->pos.col-'a'];
        case Type::N:
            if (p->color == Color::WHITE) {
                return p->points + knight_points[8-p->pos.row][p->pos.col-'a'];
            }
            return p->points + knight_points[p->pos.row-1][p->pos.col-'a'];
        case Type::P:
            if (p->color == Color::WHITE) {
                return p->points + pawns_points[8-p->pos.row][p->pos.col-'a'];
            }
            return p->points + pawns_points[p->pos.row-1][p->pos.col-'a'];
        default:
            assert("Invalid piece on the board");
            return false;
    }
    return 0;
}

std::string Engine::generate_move() {
    std::pair<std::string, int> best = std::make_pair("", 0);
    Color next_color = (main_color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    int alpha = -INFINITY, beta = INFINITY;

    Game *temp_game = new Game(game);
    if (main_color == Color::WHITE) {
        best.second = -INFINITY;
        for (std::string move_string : temp_game->possible_moves_for(main_color)) {
            temp_game->move(move_string, false);
            int rating = rate_move(temp_game, default_depth-1, alpha, beta, next_color);
            if (rating > best.second) {
                best.second = rating;
                best.first = move_string;
            }
            alpha = std::max(alpha, rating);
            temp_game->undo_move();
            if (beta < alpha) {
                return best.first;
            }
        }
    } else {
        best.second = INFINITY;
        for (std::string move_string : temp_game->possible_moves_for(main_color)) {
            temp_game->move(move_string, false);
            int rating = rate_move(temp_game, default_depth-1, alpha, beta, next_color);
            if (rating < best.second) {
                best.second = rating;
                best.first = move_string;
            }
            beta = std::min(beta, rating);
            temp_game->undo_move();
            if (beta < alpha) {
                return best.first;
            }
        }
    }
    temp_game->clear_move_queue();
    delete temp_game;
    
    return best.first;
}

int Engine::rate_move(Game *temp_game, int depth, int alpha, int beta, Color color) {
    if (temp_game->ended() || depth == 0) {
        int eval = evaluate(temp_game);
        if (eval == INFINITY) {
            eval += depth;
        } else if (eval == -INFINITY) {
            eval -= depth;
        }
        return eval;
    }
    Color next_color = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    int best_rating;

    if (color == Color::WHITE) {
        best_rating = -INFINITY;
        for (std::string move_string : temp_game->possible_moves_for(color)) {
            temp_game->move(move_string, false);
            int rating = rate_move(temp_game, depth-1, alpha, beta, next_color);
            best_rating = std::max(best_rating, rating);
            alpha = std::max(alpha, rating);
            temp_game->undo_move();
            if (beta < alpha) {
                return best_rating;
            }
        }
    } else {
        best_rating = INFINITY;
        for (std::string move_string : temp_game->possible_moves_for(color)) {
            temp_game->move(move_string, false);
            int rating = rate_move(temp_game, depth-1, alpha, beta, next_color);
            best_rating = std::min(best_rating, rating);
            beta = std::min(beta, rating);
            temp_game->undo_move();
            if (beta < alpha) {
                return best_rating;
            }
        }
    }

    return best_rating;
}
