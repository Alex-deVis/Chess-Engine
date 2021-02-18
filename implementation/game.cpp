#include "../skeleton/game.h"

Game::Game() {
    this->board = new Board();
    this->played_moves = new std::vector<Transition>();
    this->white_to_move = true;
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

Transition Game::generate_transition(std::string move_string) {
    Position start = Position(move_string[0], atoi(move_string.substr(1, 1).c_str()));
    Position end = Position(move_string[2], atoi(move_string.substr(3, 1).c_str()));
    bool capture = false, promotion = false, castle = false;
    Piece *tp = nullptr;
    if (board->is_piece_at(end)) {
        capture = true;
        tp = board->get_piece_at(end);
    }
    Piece *p = board->get_piece_at(start);
    if (p->type == Type::P && ((end.row == 8 && p->color == Color::WHITE) || (end.row == 1 && p->color == Color::BLACK))) {
        promotion = true;
    }
    if (p->type == Type::K && !has_moved(p) && (end.col == 'c' || end.col == 'g')) {
        castle = true;
    }
    return Transition(move_string, capture, promotion, castle, tp);
}

void Game::move(std::string move_string, bool forced) {
    assert(("Move length must be equal to 4." && move_string.length() == 4));

    Position start = Position(move_string[0], atoi(move_string.substr(1, 1).c_str()));
    Position end = Position(move_string[2], atoi(move_string.substr(3, 1).c_str()));

    if (!forced) {
        if (valid_move(start, end)) {
            played_moves->push_back(generate_transition(move_string));
            white_to_move = !white_to_move;
        } else {
            std::cout << "Invalid move\n";
            return;
        }
    }

    if (board->is_piece_at(start)) {
        board->get_piece_at(start)->pos = end;
        board->set_piece_at(board->get_piece_at(start), end);
        board->set_piece_at(nullptr, start);
    }
}

bool Game::valid_move(Position start, Position end) {
    // Not null move
    if (start == end) {
        std::cout << "Start == end error\n";
        return false;
    }
    Color color;
    Piece *piece;
    // Present piece
    if (!board->is_piece_at(start)) {
        std::cout << "No piece at start\n";
        return false;
    } else {
        piece = board->get_piece_at(start);
    }
    // Turn check
    color = piece->color;
    if ((white_to_move && color != Color::WHITE) || (!white_to_move && color == Color::WHITE)) {
        std::cout << "Not your turn\n";
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
        std::cout << "Piece at " << start << " cannot reach " << end << "\n";
        return false;
    }
    // In check verification

    return true;
}

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
        if (end.col == 'c') {
            Piece *rook = board->get_piece_at(Position('a', p->pos.row));
            if (rook && !has_moved(rook)) {
                if (!board->is_piece_at(Position('b', p->pos.row)) && 
                    !board->is_piece_at(Position('c', p->pos.row)) && 
                    !board->is_piece_at(Position('d', p->pos.row))) {
                        return true;
                    }
            }
        }
        if (end.col == 'g') {
            Piece *rook = board->get_piece_at(Position('h', p->pos.row));
            if (rook && !has_moved(rook)) {
                if (!board->is_piece_at(Position('f', p->pos.row)) && 
                    !board->is_piece_at(Position('g', p->pos.row))) {
                        return true;
                    }
            }
        }
    }
    return false;
}

bool Game::can_reach(Queen *p, Position end) {
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
    }

    if (board->is_piece_at(end) && board->get_piece_at(end)->color == p->color) {
        return false;
    }
    return true;
}

bool Game::can_reach(Rook *p, Position end) {
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
    }

    if (board->is_piece_at(end) && board->get_piece_at(end)->color == p->color) {
        return false;
    }
    return true;
}

bool Game::can_reach(Bishop *p, Position end) {
    if (abs(p->pos.col - end.col) == abs(p->pos.row - end.row)) {
        int coef_col = (end.col - p->pos.col > 0) ? 1 : -1;
        int coef_row = (end.row - p->pos.row > 0) ? 1 : -1;
        for (int k = 1; k < abs(p->pos.row - end.row); k++) {
            if (board->is_piece_at(Position(p->pos.col + k * coef_col, p->pos.row + k * coef_row))) {
                return false;
            }
        }
    }

    if (board->is_piece_at(end) && board->get_piece_at(end)->color == p->color) {
        return false;
    }
    return true;
}

bool Game::can_reach(Knight *p, Position end) {
    if ((abs(end.col - p->pos.col) == 1 && abs(end.row - p->pos.row) == 2) &&
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
        if (end.row - p->pos.row == 2 && !has_moved(p) && !board->is_piece_at(end) && !board->is_piece_at(Position(end.col, end.row - 1))) {
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
            ((abs(end.col - p->pos.col) == 1 && board->is_piece_at(end) && board->get_piece_at(end)->color == Color::BLACK) ||
                (end.col == p->pos.col && !board->is_piece_at(end)))) {
                    return true;
            }
        if (p->pos.row - end.row == 2 && !has_moved(p) && !board->is_piece_at(end) && !board->is_piece_at(Position(end.col, end.row + 1))) {
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

bool Game::has_moved(Piece *p) {
    for (auto x : *played_moves) {
        if (x.end == p->pos) {
            return true;
        }
    }
    return false;
}

void Game::print_board() {
    this->board->print_board();
}

bool Game::moving_linearly(Position start, Position end) {
    return (start.col == end.col || start.row == end.row);
}

bool Game::moving_diagonaly(Position start, Position end) {
    return abs(start.col-end.col) == abs(start.row-end.row);
}

