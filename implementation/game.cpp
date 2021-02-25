#include "../skeleton/game.h"

Game::Game() {
    this->board = new Board();
    this->played_moves = new std::vector<Transition>();
    this->white_to_move = true;
    this->taken_white = 0;
    this->taken_black = 0;
}

Game::Game(Game *game) {
    this->white_to_move = game->white_to_move;
    this->taken_white = game->taken_white;
    this->taken_black = game->taken_black;
    this->board = new Board(game->board);
    this->played_moves = new std::vector<Transition>();
}

Game::~Game() {
    delete this->board;
    for (Transition t : *played_moves) {
        if (t.taken_piece != nullptr) {
            delete t.taken_piece;
        }
    }
    delete this->played_moves;
}

void Game::turn(Color color) {
    white_to_move = color == Color::WHITE;
}

// Decides the side effects for a valid move
Transition Game::generate_transition(std::string move_string) {
    Position start = Position(move_string[0], atoi(move_string.substr(1, 1).c_str()));
    Position end = Position(move_string[2], atoi(move_string.substr(3, 1).c_str()));
    bool promotion = false, castle = false, enPassant = false;
    Piece *tp = nullptr;
    if (board->is_piece_at(end)) {
        tp = board->get_piece_at(end);
    }
    Piece *p = board->get_piece_at(start);
    if (p->type == Type::P && ((end.row == 8 && p->color == Color::WHITE) || (end.row == 1 && p->color == Color::BLACK))) {
        promotion = true;
    }
    if (p->type == Type::K && !has_moved(p) && (end.col == 'c' || end.col == 'g')) {
        castle = true;
    }
    if (p->type == Type::P && !played_moves->empty()) {
        Transition tr = played_moves->back();
        if (abs(tr.start.row - tr.end.row) == 2 && end.col == tr.end.col && start.row == tr.end.row &&
                p->color != board->get_piece_at(played_moves->back().end)->color) {
            enPassant = true;
            tp = board->get_piece_at(played_moves->back().end);
        }
    }
    return Transition(move_string, promotion, castle, enPassant, false, tp);
}

void Game::move(std::string move_string, bool forced) {
    assert(("Move length must be equal to 4." && move_string.length() == 4));

    Position start = Position(move_string[0], atoi(move_string.substr(1, 1).c_str()));
    Position end = Position(move_string[2], atoi(move_string.substr(3, 1).c_str()));

    if (!forced && valid_move(start, end)) {
        Transition config = generate_transition(move_string);
        // Transport the piece to the desired square
        if (config.castle) {
            Piece *r = board->get_piece_at(Position((end.col == 'c') ? 'a' : 'h', end.row));
            Piece *k = board->get_piece_at(start);

            Position r_pos((r->pos.col == 'a') ? 'd' : 'f', end.row);
            board->move_piece(k, end);
            board->move_piece(r, r_pos);
        } else if (config.promotion) {
            // Promote to queen by default
            board->set_piece_at(new Queen(board->get_piece_at(start)->color, end), end);
            delete board->get_piece_at(start);
            board->set_piece_at(nullptr, start);
        } else {
            if (config.enPassant) {
                board->set_piece_at(nullptr, played_moves->back().end);
            }
            board->move_piece(board->get_piece_at(start), end);
        }
        if (config.taken_piece != nullptr) {
            if (config.taken_piece->color == Color::WHITE) {
                taken_white++;
            } else {
                taken_black++;
            }
        }
        played_moves->push_back(config);
        white_to_move = !white_to_move;
    } else if (forced && board->is_piece_at(start)) {
        if (board->get_piece_at(start)->type == Type::K) {
            if (board->get_piece_at(start)->color == Color::WHITE) {
                board->w_king = end;
            } else {
                board->b_king = end;
            }
        }
        Transition t(move_string, false, false, false, true, board->get_piece_at(end));
        if (t.taken_piece != nullptr) {
            if (t.taken_piece->color == Color::WHITE) {
                taken_white++;
            } else {
                taken_black++;
            }
        }
        played_moves->push_back(t);
        board->move_piece(board->get_piece_at(start), end);
    }
}

/* Move checking*/
bool Game::valid_move(Position start, Position end, bool suppress_msg) {
    // Not null move
    if (start == end) {
        if (!suppress_msg) std::cout << "Start == End error\n";
        return false;
    }
    Piece *piece;
    // Present piece
    if (!board->is_piece_at(start)) {
        if (!suppress_msg) std::cout << "No piece at start\n";
        return false;
    } else {
        piece = board->get_piece_at(start);
    }
    // Turn check
    Color to_move = (white_to_move) ? Color::WHITE : Color::BLACK;

    if (piece->color != to_move) {
        if (!suppress_msg) std::cout << "Not your turn\n";
        return false;
    }
    // Reachable
    bool canReach;
    switch (piece->type) {
        case K:
            canReach = can_reach((King*)piece, end);
            break;
        case Q:
            canReach = can_reach((Queen*)piece, end);
            break;
        case R:
            canReach = can_reach((Rook*)piece, end);
            break;
        case B:
            canReach = can_reach((Bishop*)piece, end);
            break;
        case N:
            canReach = can_reach((Knight*)piece, end);
            break;
        case P:
            canReach = can_reach((Pawn*)piece, end);
            break;
        default:
            assert("Invalid piece on the board");
            return false;
    }
    if (canReach == false) {
        if (!suppress_msg) std::cout << "Piece at " << start << " cannot reach " << end << "\n";
        return false;
    }

    // In check verification
    if (will_remain_in_check_after(start, end)) {
        if (!suppress_msg) (board->get_piece_at(start)->color == Color::WHITE) ? std::cout << "White " : std::cout << "Black "; 
        if (!suppress_msg) std::cout << " will remain in check if that move is played\n";
        return false;
    }

    return true;
}

// Move patterns verification
bool Game::can_reach(King *p, Position end) {
    if (abs(p->pos.col - end.col) <= 1 && abs(p->pos.row - end.row) <= 1) {
        if (board->is_piece_at(end)) {
            Piece *target = board->get_piece_at(end);
            if (target->color == p->color) {
                return false;
            }
        }
        return true;
    } else if (!has_moved(p)) {
        Color enemy_color = (p->color == Color::WHITE) ? Color::BLACK : Color::WHITE;
        if (end.col == 'c') {  // King side castle
            Piece *rook = board->get_piece_at(Position('a', p->pos.row));
            if (rook && !has_moved(rook)) {
                if (!board->is_piece_at(Position('b', p->pos.row)) && 
                    !board->is_piece_at(Position('c', p->pos.row)) && 
                    !board->is_piece_at(Position('d', p->pos.row)) &&
                    !can_color_reach(enemy_color, Position('c', p->pos.row)) &&
                    !can_color_reach(enemy_color, Position('d', p->pos.row)) &&
                    !can_color_reach(enemy_color, Position('e', p->pos.row))) {
                        return true;
                    }
            }
        }
        if (end.col == 'g') {
            Piece *rook = board->get_piece_at(Position('h', p->pos.row));
            if (rook && !has_moved(rook)) {
                if (!board->is_piece_at(Position('f', p->pos.row)) && 
                    !board->is_piece_at(Position('g', p->pos.row)) &&
                    !can_color_reach(enemy_color, Position('e', p->pos.row)) &&
                    !can_color_reach(enemy_color, Position('f', p->pos.row)) &&
                    !can_color_reach(enemy_color, Position('g', p->pos.row))) {
                        return true;
                    }
            }
        }
    }
    return false;
}
bool Game::can_reach(Queen *p, Position end) {
    if (board->is_piece_at(end) && board->get_piece_at(end)->color == p->color) {
        return false;
    }
    if (p->pos.row == end.row) {
        for (int k = 1; k < abs(p->pos.col - end.col); k++) {
            if (board->is_piece_at(Position(std::min(p->pos.col, end.col) + k, end.row))) {
                return false;
            }
        }
    } else if (p->pos.col == end.col) {
        for (int k = 1; k < abs(p->pos.row - end.row); k++) {
            if (board->is_piece_at(Position(end.col, std::min(p->pos.row, end.row) + k))) {
                return false;
            }
        }
    } else if (abs(p->pos.col - end.col) == abs(p->pos.row - end.row)) {
        int coef_col = (end.col - p->pos.col > 0) ? 1 : -1;
        int coef_row = (end.row - p->pos.row > 0) ? 1 : -1;
        for (int k = 1; k < abs(p->pos.row - end.row); k++) {
            if (board->is_piece_at(Position(p->pos.col + k * coef_col, p->pos.row + k * coef_row))) {
                return false;
            }
        }
    } else {
        return false;
    }

    return true;;
}
bool Game::can_reach(Rook *p, Position end) {
    if (board->is_piece_at(end) && board->get_piece_at(end)->color == p->color) {
        return false;
    }
    if (p->pos.row == end.row) {
        for (int k = 1; k < abs(p->pos.col - end.col); k++) {
            if (board->is_piece_at(Position(std::min(p->pos.col, end.col) + k, end.row))) {
                return false;
            }
        }
    } else if (p->pos.col == end.col) {
        for (int k = 1; k < abs(p->pos.row - end.row); k++) {
            if (board->is_piece_at(Position(end.col, std::min(p->pos.row, end.row) + k))) {
                return false;
            }
        }
    } else {
        return false;
    }
    
    return true;
}
bool Game::can_reach(Bishop *p, Position end) {
    if (board->is_piece_at(end) && board->get_piece_at(end)->color == p->color) {
        return false;
    }
    if (abs(p->pos.col - end.col) == abs(p->pos.row - end.row)) {
        int coef_col = (end.col - p->pos.col > 0) ? 1 : -1;
        int coef_row = (end.row - p->pos.row > 0) ? 1 : -1;
        for (int k = 1; k < abs(p->pos.row - end.row); k++) {
            if (board->is_piece_at(Position(p->pos.col + k * coef_col, p->pos.row + k * coef_row))) {
                return false;
            }
        }
    } else {
        return false;
    }

    return true;
}
bool Game::can_reach(Knight *p, Position end) {
    if ((abs(end.col - p->pos.col) == 1 && abs(end.row - p->pos.row) == 2) ||
            (abs(end.col - p->pos.col) == 2 && abs(end.row - p->pos.row) == 1)) {
        if (board->is_piece_at(end) && board->get_piece_at(end)->color == p->color) {
            return false;
        }
        return true;
    }
    return false;
}
bool Game::can_reach(Pawn *p, Position end) {
    if (p->color == Color::WHITE) {
        if ((end.row - p->pos.row == 1) &&
            ((abs(end.col - p->pos.col) == 1 && board->is_piece_at(end) && board->get_piece_at(end)->color == Color::BLACK) ||
                (end.col == p->pos.col && !board->is_piece_at(end)))) {
                    return true;
            }
        if (end.row - p->pos.row == 2 && end.col == p->pos.col && !has_moved(p) &&
                !board->is_piece_at(end) && !board->is_piece_at(Position(end.col, end.row - 1))) {
            return true;
        }
        // Takes eps
        if (p->pos.row == 5 && end.row == 6 && abs(end.col - p->pos.col) == 1) {
            Piece *last_moved = board->get_piece_at(played_moves->back().end);
            if (last_moved->type == Type::P && played_moves->back().start.row == 7 && played_moves->back().end.row == 5) {
                return true;
            } 
        }
    } else {
        if ((p->pos.row - end.row == 1) &&
            ((abs(end.col - p->pos.col) == 1 && board->is_piece_at(end) && board->get_piece_at(end)->color == Color::WHITE) ||
                (end.col == p->pos.col && !board->is_piece_at(end)))) {
                    return true;
            }
        if (p->pos.row - end.row == 2 && end.col == p->pos.col && !has_moved(p) &&
                !has_moved(p) && !board->is_piece_at(end) && !board->is_piece_at(Position(end.col, end.row + 1))) {
            return true;
        }
        // Takes eps
        if (p->pos.row == 4 && end.row == 3 && abs(end.col - p->pos.col) == 1) {
            Piece *last_moved = board->get_piece_at(played_moves->back().end);
            if (last_moved->type == Type::P && played_moves->back().start.row == 2 && played_moves->back().end.row == 4) {
                return true;
            } 
        }
    }
    return false;
}

// Avoid illegal moves that will leave a player in check
bool Game::will_remain_in_check_after(Position start, Position end) {
    if (start == Position('d', 6) && end == Position('f', 7)) {
        std::cout << "BEFORE: " << board->b_king << "\n";
        print_board();
    }

    bool in_check = false;
    Color color = board->get_piece_at(start)->color;
    Color enemy_color = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    Position king_pos = (color == Color::WHITE) ? board->w_king : board->b_king;
    // Simulate move
    if (board->get_piece_at(start)->type == Type::K) {
        king_pos = end;
    }
    Piece *taken_piece = board->get_piece_at(end);
    board->move_piece(board->get_piece_at(start), end);

    in_check = can_color_reach(enemy_color, king_pos);
    if (start == Position('d', 6) && end == Position('f', 7) && in_check) {
        std::cout << "AFTER\n";
        print_board();
    }

    board->move_piece(board->get_piece_at(end), start);
    board->move_piece(taken_piece, end);
    return in_check;
}

// Verify if a square is currently controlled by enemy pieces
bool Game::can_color_reach(Color color, Position target) {
    /* Verifying if enemy pieces can reach the position */
    Piece *encountered_piece;
    Piece *target_piece = board->get_piece_at(target);
    if (target_piece && target_piece->color == color) {
        return false;
    }
    // Pawn checking
    if (color == WHITE) {
        if (target_piece && target_piece->color!= color) {  // Enemy piece is target
            for (int i=-1; i<=1; i+=2) {
                if (target.col+i <= 'h' && target.col+i >= 'a' && target.row-1 >= 1) {
                    encountered_piece = board->get_piece_at(Position(target.col+i, target.row-1));
                    if (encountered_piece && encountered_piece->color == color && encountered_piece->type == Type::P) {
                        return true;
                    }
                }
            }
        } else if (!target_piece) {
            if (target.row-1 >= 1) {
                encountered_piece = board->get_piece_at(Position(target.col, target.row-1));
                if (encountered_piece && encountered_piece->color == color && encountered_piece->type == Type::P) {
                    return true;
                }
            }
        }
    } else {
        if (target_piece && target_piece->color!= color) {  // Enemy piece is target
            for (int i=-1; i<=1; i+=2) {
                if (target.col+i <= 'h' && target.col+i >= 'a' && target.row+1 <= 8) {
                    encountered_piece = board->get_piece_at(Position(target.col+i, target.row+1));
                    if (encountered_piece && encountered_piece->color == color && encountered_piece->type == Type::P) {
                        return true;
                    }
                }
            }
        } else if (!target_piece) {
            if (target.row+1 <= 8) {
                encountered_piece = board->get_piece_at(Position(target.col, target.row+1));
                if (encountered_piece && encountered_piece->color == color && encountered_piece->type == Type::P) {
                    return true;
                }
            }
        }
    }

    // Horizontally
    for (char c = target.col-1; c >= 'a'; c--) {
        encountered_piece = board->get_piece_at(Position(c, target.row));
        if (encountered_piece && encountered_piece->color == color &&
                ((encountered_piece->type == Type::Q || encountered_piece->type == Type::R))) {
            return true;
        } else if (encountered_piece && encountered_piece->color != color) {
            break;
        }
    }
    for (char c = target.col+1; c <= 'h'; c++) {
        encountered_piece = board->get_piece_at(Position(c, target.row));
        if (encountered_piece && encountered_piece->color == color &&
                ((encountered_piece->type == Type::Q || encountered_piece->type == Type::R))) {
            return true;
        } else if (encountered_piece && encountered_piece->color != color) {
            break;
        }
    }
    // Vertically
    for (int r = target.row-1; r >= 1; r--) {
        encountered_piece = board->get_piece_at(Position(target.col, r));
        if (encountered_piece && encountered_piece->color == color &&
                ((encountered_piece->type == Type::Q || encountered_piece->type == Type::R))) {
            return true;
        } else if (encountered_piece && encountered_piece->color != color) {
            break;
        }
    }
    for (int r = target.row+1; r <= 8; r++) {
        encountered_piece = board->get_piece_at(Position(target.col, r));
        if (encountered_piece && encountered_piece->color == color &&
                ((encountered_piece->type == Type::Q || encountered_piece->type == Type::R))) {
            return true;
        } else if (encountered_piece && encountered_piece->color != color) {
            break;
        }
    }
    // Diagonally
    int coef_col[] = {1, 1, -1, -1};
    int coef_row[] = {1, -1, -1, 1};
    
    for (int k=0; k<4; k++) {
        int dist;
        if (coef_col[k] > 0) {
            dist = 'h'-target.col;
        } else {
            dist = target.col-'a';
        }
        if (coef_row[k] > 0) {
            dist = std::min(dist, 8-target.row);
        } else {
            dist = std::min(dist, target.row-1);
        }

        for (int d=1; d<=dist; d++) {
            encountered_piece = board->get_piece_at(Position(target.col + d*coef_col[k], target.row + d*coef_row[k]));
            if (encountered_piece && encountered_piece->color == color &&
                ((encountered_piece->type == Type::Q || encountered_piece->type == Type::B))) {
                return true;
            } else if (encountered_piece && encountered_piece->color != color) {
                break;
            }
        }
    }
    // Knight verification
    return  is_knight_at(target, -2, -1, color) || is_knight_at(target, -2, 1, color) ||
            is_knight_at(target, -1, -2, color) || is_knight_at(target, -1, 2, color) ||
            is_knight_at(target, 1, -2, color) || is_knight_at(target, 1, 2, color) ||
            is_knight_at(target, 2, -1, color) || is_knight_at(target, 2, 1, color);
}

// Checks if a knight of specified color is at given offset from a position
bool Game::is_knight_at(Position pos, int col_off, int row_off, Color color) {
    Piece *p;
    if (pos.col+col_off >= 'a' && pos.col+col_off <= 'h' && pos.row+row_off >= 1 && pos.row+row_off <= 8) {
        p = board->get_piece_at(Position(pos.col+col_off, pos.row+row_off));
        return (p && p->color == color && p->type == Type::N);
    }
    return false;
}

// Undo the last move
void Game::undo_move() {
    if (!played_moves->empty()) {
        Transition t = played_moves->back();
        if (t.forced) {
            board->move_piece(board->get_piece_at(t.end), t.start);
            board->move_piece(t.taken_piece, t.end);
        } else if (t.castle) {
            board->move_piece(board->get_piece_at(t.end), t.start);
            Piece *r = board->get_piece_at(Position((t.end.col == 'c') ? 'd' : 'g', t.end.row));
            board->move_piece(r, Position((r->pos.col == 'd') ? 'a' : 'h', r->pos.row));
        } else if (t.promotion) {
            Pawn *pawn = new Pawn(board->get_piece_at(t.end)->color, t.end);
            delete board->get_piece_at(t.end);
            board->move_piece(pawn, t.start);
            board->move_piece(t.taken_piece, t.end);
        } else if (t.enPassant) {
            board->move_piece(board->get_piece_at(t.end), t.start);
            board->move_piece(t.taken_piece, t.taken_piece->pos);
        } else {
            board->move_piece(board->get_piece_at(t.end), t.start);
            board->move_piece(t.taken_piece, t.end);
        }
        played_moves->pop_back();
        if (!t.forced) { white_to_move = !white_to_move; }
        if (t.taken_piece) {
            if (t.taken_piece->color == Color::WHITE) {
                taken_white--;
            } else {
                taken_black--;
            }
        }
    }
}

// Verifies if the piece moved
bool Game::has_moved(Piece *p) {
    for (auto x : *played_moves) {
        if (x.end == p->pos) {
            return true;
        }
    }
    return false;
}

// Prints current configuration of the board
void Game::print_board() {
    this->board->print_board();
}

std::vector<std::string> Game::possible_moves_for(Color color) {
    std::vector<std::string> pos_moves;
    std::vector<std::string> aux;
    Piece *p;
    if (color == Color::WHITE) {
        int ct=taken_white;
        for (int r=1; r<=8; r++) {
            for (char c='a'; c<='h'; c++) {
                p = board->get_piece_at(Position(c, r));
                if (p && p->color == Color::WHITE) {
                    switch (p->type) {
                        case K:
                            aux = possible_moves_for((King*)p);
                            break;
                        case Q:
                            aux = possible_moves_for((Queen*)p);
                            break;
                        case R:
                            aux = possible_moves_for((Rook*)p);
                            break;
                        case B:
                            aux = possible_moves_for((Bishop*)p);
                            break;
                        case N:
                            aux = possible_moves_for((Knight*)p);
                            break;
                        case P:
                            aux = possible_moves_for((Pawn*)p);
                            break;
                        default:
                            assert("Invalid piece on the board");
                            return std::vector<std::string>();
                    }
                    for (std::string s : aux) {
                        pos_moves.push_back(s);
                    }
                    if (++ct == 16) {
                        break;
                    }
                }
            }
        }
    } else {
        int ct=taken_black;
        for (int r=8; r>=1; r--) {
            for (char c='a'; c<='h'; c++) {
                p = board->get_piece_at(Position(c, r));
                if (p && p->color == Color::BLACK) {
                    switch (p->type) {
                        case K:
                            aux = possible_moves_for((King*)p);
                            break;
                        case Q:
                            aux = possible_moves_for((Queen*)p);
                            break;
                        case R:
                            aux = possible_moves_for((Rook*)p);
                            break;
                        case B:
                            aux = possible_moves_for((Bishop*)p);
                            break;
                        case N:
                            aux = possible_moves_for((Knight*)p);
                            break;
                        case P:
                            aux = possible_moves_for((Pawn*)p);
                            break;
                        default:
                            assert("Invalid piece on the board");
                            return std::vector<std::string>();
                    }
                    for (std::string s : aux) {
                        pos_moves.push_back(s);
                    }
                    if (++ct == 16) {
                        break;
                    }
                }
            }
        }
    }

    return pos_moves;
}

bool Game::are_possible_moves_for(Color color) {
    std::vector<std::string> pos_moves;
    std::vector<std::string> aux;
    Piece *p;
    if (color == Color::WHITE) {
        int ct=taken_white;
        for (int r=1; r<=8; r++) {
            for (char c='a'; c<='h'; c++) {
                p = board->get_piece_at(Position(c, r));
                if (p && p->color == Color::WHITE) {
                    switch (p->type) {
                        case K:
                            aux = possible_moves_for((King*)p);
                            break;
                        case Q:
                            aux = possible_moves_for((Queen*)p);
                            break;
                        case R:
                            aux = possible_moves_for((Rook*)p);
                            break;
                        case B:
                            aux = possible_moves_for((Bishop*)p);
                            break;
                        case N:
                            aux = possible_moves_for((Knight*)p);
                            break;
                        case P:
                            aux = possible_moves_for((Pawn*)p);
                            break;
                        default:
                            assert("Invalid piece on the board");
                            return false;
                    }
                    if (!aux.empty()) {
                        return true;
                    }
                    if (++ct == 16) {
                        break;
                    }
                }
            }
        }
    } else {
        int ct=taken_black;
        for (int r=8; r>=1; r--) {
            for (char c='a'; c<='h'; c++) {
                p = board->get_piece_at(Position(c, r));
                if (p && p->color == Color::BLACK) {
                    switch (p->type) {
                        case K:
                            aux = possible_moves_for((King*)p);
                            break;
                        case Q:
                            aux = possible_moves_for((Queen*)p);
                            break;
                        case R:
                            aux = possible_moves_for((Rook*)p);
                            break;
                        case B:
                            aux = possible_moves_for((Bishop*)p);
                            break;
                        case N:
                            aux = possible_moves_for((Knight*)p);
                            break;
                        case P:
                            aux = possible_moves_for((Pawn*)p);
                            break;
                        default:
                            assert("Invalid piece on the board");
                            return false;
                    }
                    if (!aux.empty()) {
                        return true;
                    }
                    if (++ct == 16) {
                        break;
                    }
                }
            }
        }
    }
    return false;
}

std::vector<std::string> Game::possible_moves_for(King *p) {
    std::vector<std::string> pos_moves;
    std::string base = std::string(1, p->pos.col) + std::to_string(p->pos.row);

    for (int i=-1; i<=1; i++) {
        for (int j=-1; j<=1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            if (p->pos.col+i <= 'h' && p->pos.col+i >= 'a' && p->pos.row+j <= 8 && p->pos.row+j >= 1) {
                if (valid_move(p->pos, Position(p->pos.col+i, p->pos.row+j), true)) {
                    std::string move_string = base + std::string(1, p->pos.col+i) + std::to_string(p->pos.row+j);
                    pos_moves.push_back(move_string);
                }
            }
        }
    }
    if (!has_moved(p)) {
        if (valid_move(p->pos, Position('c', p->pos.row), true)) {
            std::string move_string = base + std::string(1, 'c') + std::to_string(p->pos.row);
            pos_moves.push_back(move_string);
        }
        if (valid_move(p->pos, Position('g', p->pos.row), true)) {
            std::string move_string = base + std::string(1, 'g') + std::to_string(p->pos.row);
            pos_moves.push_back(move_string);
        }
    }

    return pos_moves;
}
std::vector<std::string> Game::possible_moves_for(Queen *p) {
    std::vector<std::string> pos_moves;
    std::string base = std::string(1, p->pos.col) + std::to_string(p->pos.row);

    // Horizontally
    for (char c=p->pos.col-1; c>='a'; c--) {
        if (valid_move(p->pos, Position(c, p->pos.row), true)) {
            std::string move_string = base + std::string(1, c) + std::to_string(p->pos.row);
            pos_moves.push_back(move_string);
        }
        if (board->is_piece_at(Position(c, p->pos.row))) {
            break;
        }
    }
    for (char c=p->pos.col+1; c<='h'; c++) {
        if (valid_move(p->pos, Position(c, p->pos.row), true)) {
            std::string move_string = base + std::string(1, c) + std::to_string(p->pos.row);
            pos_moves.push_back(move_string);
        }
        if (board->is_piece_at(Position(c, p->pos.row))) {
            break;
        }
    }
    // Vertically
    for (int r=p->pos.row-1; r>=1; r--) {
        if (valid_move(p->pos, Position(p->pos.col, r), true)) {
            std::string move_string = base + std::string(1, p->pos.col) + std::to_string(r);
            pos_moves.push_back(move_string);
        }
        if (board->is_piece_at(Position(p->pos.col, r))) {
            break;
        }
    }
    for (int r=p->pos.row+1; r<=8; r++) {
        if (valid_move(p->pos, Position(p->pos.col, r), true)) {
            std::string move_string = base + std::string(1, p->pos.col) + std::to_string(r);
            pos_moves.push_back(move_string);
        }
        if (board->is_piece_at(Position(p->pos.col, r))) {
            break;
        }
    }
    // Diagonally
    int coef_col[] = {1, 1, -1, -1};
    int coef_row[] = {1, -1, -1, 1};
    for (int k=0; k<4; k++) {
        for (int d=1; d<=7; d++) {
            if (p->pos.col+coef_col[k]*d >= 'a' && p->pos.col+coef_col[k]*d <= 'h'
                    && p->pos.row+coef_row[k]*d >= 1 && p->pos.row+coef_row[k]*d <= 8) {
                Position tmp(p->pos.col+coef_col[k]*d, p->pos.row+coef_row[k]*d);
                if (valid_move(p->pos, tmp, true)) {
                    std::string move_string = base + std::string(1, tmp.col) + std::to_string(tmp.row);
                    pos_moves.push_back(move_string);
                }
                if (board->is_piece_at(tmp)) {
                    break;
                }
            }
        }
    }

    return pos_moves;
}
std::vector<std::string> Game::possible_moves_for(Rook *p) {
    std::vector<std::string> pos_moves;
    std::string base = std::string(1, p->pos.col) + std::to_string(p->pos.row);

    // Horizontally
    for (char c=p->pos.col-1; c>='a'; c--) {
        if (valid_move(p->pos, Position(c, p->pos.row), true)) {
            std::string move_string = base + std::string(1, c) + std::to_string(p->pos.row);
            pos_moves.push_back(move_string);
        }
        if (board->is_piece_at(Position(c, p->pos.row))) {
            break;
        }
    }
    for (char c=p->pos.col+1; c<='h'; c++) {
        if (valid_move(p->pos, Position(c, p->pos.row), true)) {
            std::string move_string = base + std::string(1, c) + std::to_string(p->pos.row);
            pos_moves.push_back(move_string);
        }
        if (board->is_piece_at(Position(c, p->pos.row))) {
            break;
        }
    }
    // Vertically
    for (int r=p->pos.row-1; r>=1; r--) {
        if (valid_move(p->pos, Position(p->pos.col, r), true)) {
            std::string move_string = base + std::string(1, p->pos.col) + std::to_string(r);
            pos_moves.push_back(move_string);
        }
        if (board->is_piece_at(Position(p->pos.col, r))) {
            break;
        }
    }
    for (int r=p->pos.row+1; r<=8; r++) {
        if (valid_move(p->pos, Position(p->pos.col, r), true)) {
            std::string move_string = base + std::string(1, p->pos.col) + std::to_string(r);
            pos_moves.push_back(move_string);
        }
        if (board->is_piece_at(Position(p->pos.col, r))) {
            break;
        }
    }
    return pos_moves;
}
std::vector<std::string> Game::possible_moves_for(Bishop *p) {
    std::vector<std::string> pos_moves;
    std::string base = std::string(1, p->pos.col) + std::to_string(p->pos.row);

    // Diagonally
    int coef_col[] = {1, 1, -1, -1};
    int coef_row[] = {1, -1, -1, 1};
    for (int k=0; k<4; k++) {
        for (int d=1; d<=7; d++) {
            if (p->pos.col+coef_col[k]*d >= 'a' && p->pos.col+coef_col[k]*d <= 'h'
                    && p->pos.row+coef_row[k]*d >= 1 && p->pos.row+coef_row[k]*d <= 8) {
                Position tmp(p->pos.col+coef_col[k]*d, p->pos.row+coef_row[k]*d);
                if (valid_move(p->pos, tmp, true)) {
                    std::string move_string = base + std::string(1, tmp.col) + std::to_string(tmp.row);
                    pos_moves.push_back(move_string);
                }
                if (board->is_piece_at(tmp)) {
                    break;
                }
            }
        }
    }

    return pos_moves;
}
std::vector<std::string> Game::possible_moves_for(Knight *p) {
    std::vector<std::string> pos_moves;
    std::string base = std::string(1, p->pos.col) + std::to_string(p->pos.row);

    int col_off[] = {-2, -1, 1, 2};
    for (int k=0; k<4; k++) {
        if (p->pos.col+col_off[k] <= 'h' && p->pos.col+col_off[k] >= 'a') {
            int row_off = 3 - std::abs(col_off[k]);
            for (int r=-row_off; r<=row_off; r+=row_off*2) {
                if (p->pos.row+r <= 8 && p->pos.row+r >= 1) {
                    if (valid_move(p->pos, Position(p->pos.col+col_off[k], p->pos.row+r), true)) {
                        std::string move_string = base + std::string(1, p->pos.col+col_off[k]) + std::to_string(p->pos.row+r);
                        pos_moves.push_back(move_string);
                    }
                }
            }
        }
    }

    return pos_moves;
}
std::vector<std::string> Game::possible_moves_for(Pawn *p) {
    std::vector<std::string> pos_moves;
    std::string base = std::string(1, p->pos.col) + std::to_string(p->pos.row);

    if (p->color == Color::WHITE) {
        for (int i=-1; i<=1; i++) {
            if (p->pos.col+i >= 'a' && p->pos.col+i <='h') {
                if (valid_move(p->pos, Position(p->pos.col+i, p->pos.row+1), true)) {
                    std::string move_string = base + std::string(1, p->pos.col+i) + std::to_string(p->pos.row+1);
                    pos_moves.push_back(move_string);
                }
            }
        }
        if (!has_moved(p) && valid_move(p->pos, Position(p->pos.col, p->pos.row+2), true)) {
            std::string move_string = base + std::string(1, p->pos.col) + std::to_string(p->pos.row+2);
            pos_moves.push_back(move_string);
        }
    } else {
        for (int i=-1; i<=1; i++) {
            if (p->pos.col+i >= 'a' && p->pos.col+i <='h') {
                if (valid_move(p->pos, Position(p->pos.col+i, p->pos.row-1), true)) {
                    std::string move_string = base + std::string(1, p->pos.col+i) + std::to_string(p->pos.row-1);
                    pos_moves.push_back(move_string);
                }
            }
        }
        if (!has_moved(p) && valid_move(p->pos, Position(p->pos.col, p->pos.row-2), true)) {
            std::string move_string = base + std::string(1, p->pos.col) + std::to_string(p->pos.row-2);
            pos_moves.push_back(move_string);
        }
    }

    return pos_moves;
}

double Game::evaluate() {
    if (ended()) {
        if (white_to_move) {
            // Checkmate for black
            if (can_color_reach(Color::BLACK, board->w_king)) {
                return -INFINITY;
            } else { // Stalemate
                return 0;
            }
        } else {
            // Checkmate for white
            if (can_color_reach(Color::WHITE, board->b_king)) {
                return INFINITY;
            } else {  // Stalemate
                return 0;
            }
        }
    }
    Piece *p;
    double score=0;
    int ct=taken_white+taken_black;
    for (int r=1; r<=8; r++) {
        for (char c='a'; c<='h'; c++) {
            p = board->get_piece_at(Position(c, r));
            if (p) {
                if (p->color == Color::WHITE) {
                    score += p->points;
                } else {
                    score -= p->points;
                }
                ct++;
                if (ct == 32) {
                    break;
                }
            }
        }
    }
    return score;
}

bool Game::ended() {
    if (!white_to_move && !are_possible_moves_for(Color::BLACK)) {
        print_board();
        if (valid_move(Position('d', 6), Position('f', 7))) {
            std::cout << "PL yes\n";
        }
    }
    return (white_to_move && !are_possible_moves_for(Color::WHITE)) || (!white_to_move && !are_possible_moves_for(Color::BLACK));
}

void Game::which_turn() {
    white_to_move ? std::cout << "WHITE\n" : std::cout << "BLACK\n";
}

bool Game::is_piece_at(Position pos) {
    return board->is_piece_at(pos);
}

std::string Game::last_played_move() {
    return played_moves->back().move_string;
}