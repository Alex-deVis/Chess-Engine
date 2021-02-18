bool Game::will_remain_in_check_after(Position start, Position end) {
    Piece *piece = board->get_piece_at(start);
    Color frendly_color = piece->color;
    Position king_pos = (piece->color == Color::WHITE) ? board->w_king : board->b_king;
    if (piece->type == Type::K) {
        king_pos = end;
    }
    /* Verifying if enemy pieces can reach the king */
    Piece *encountered_piece;
    // Pawn
    if (frendly_color == Color::WHITE && king_pos.row <= 6) {
        if (king_pos.col >= 'b') {
            encountered_piece = board->get_piece_at(Position(king_pos.col-1, king_pos.row+1));
            if (encountered_piece && encountered_piece->color != frendly_color && encountered_piece->type == Type::P) {
                return true;
            }
        }
        if (king_pos.col <= 'g') {
            encountered_piece = board->get_piece_at(Position(king_pos.col+1, king_pos.row+1));
            if (encountered_piece && encountered_piece->color != frendly_color && encountered_piece->type == Type::P) {
                return true;
            }
        }
    }
    if (frendly_color == Color::BLACK && king_pos.row >= 3) {
        if (king_pos.col >= 'b') {
            encountered_piece = board->get_piece_at(Position(king_pos.col-1, king_pos.row-1));
            if (encountered_piece && encountered_piece->color != frendly_color && encountered_piece->type == Type::P) {
                return true;
            }
        }
        if (king_pos.col <= 'g') {
            encountered_piece = board->get_piece_at(Position(king_pos.col+1, king_pos.row-1));
            if (encountered_piece && encountered_piece->color != frendly_color && encountered_piece->type == Type::P) {
                return true;
            }
        }
    }
    
    // Horizontally
    for (char c = king_pos.col-1; c >= 1; c--) {
        encountered_piece = board->get_piece_at(Position(c, king_pos.row));
        if (encountered_piece != nullptr) {
            if (encountered_piece->color == frendly_color) {
                break;
            } else {
                if (encountered_piece->type == Type::Q || encountered_piece->type == Type::R) {
                    return true;
                }
            }
        }
    }
    for (char c = king_pos.col+1; c <= 8; c++) {
        encountered_piece = board->get_piece_at(Position(c, king_pos.row));
        if (encountered_piece != nullptr) {
            if (encountered_piece->color == frendly_color) {
                break;
            } else {
                if (encountered_piece->type == Type::Q || encountered_piece->type == Type::R) {
                    return true;
                }
            }
        }
    }
    // Vertically
    for (int r = king_pos.row-1; r >= 1; r--) {
        encountered_piece = board->get_piece_at(Position(king_pos.col, r));
        if (encountered_piece != nullptr) {
            if (encountered_piece->color == frendly_color) {
                break;
            } else {
                if (encountered_piece->type == Type::Q || encountered_piece->type == Type::R) {
                    return true;
                }
            }
        }
    }
    for (int r = king_pos.row+1; r <= 8; r++) {
        encountered_piece = board->get_piece_at(Position(king_pos.col, r));
        if (encountered_piece != nullptr) {
            if (encountered_piece->color == frendly_color) {
                break;
            } else {
                if (encountered_piece->type == Type::Q || encountered_piece->type == Type::R) {
                    return true;
                }
            }
        }
    }
    // Diagonally
    for (int k = 1; k <= 4; k++) {
        int coef_col = k & 1, coef_row = k & 2;
        int dist;
        if (!coef_col) {
            coef_col = -1;
            dist = king_pos.col-'a';
        } else {
            dist = 'h'-king_pos.col;
        }
        if (!coef_row) {
            coef_row = -1;
            dist = std::min(dist, king_pos.row-1);
        } else {
            dist = std::min(dist, 8-king_pos.row);
        }
        for (int d = 1; d <= dist; d++) {
            encountered_piece = board->get_piece_at(Position(king_pos.col+d*coef_col, king_pos.row+d*coef_row));
            if (encountered_piece != nullptr) {
                if (encountered_piece->color == frendly_color) {
                    break;
                } else {
                    if (encountered_piece->type == Type::Q || encountered_piece->type == Type::B) {
                        return true;
                    }
                }
            }
        }
    }
    // Knight
    return knight_check(king_pos, -2, -1) || knight_check(king_pos, -2, 1) ||
            knight_check(king_pos, -1, -2) || knight_check(king_pos, -1, 2) ||
            knight_check(king_pos, 1, -2) || knight_check(king_pos, 1, 2) ||
            knight_check(king_pos, 2, -1) || knight_check(king_pos, 2, 1);
}

void Game::move(std::string move_string) {
    Position start = Position(move_string[0], atoi(move_string.substr(1, 1).c_str()));
    Position end = Position(move_string[2], atoi(move_string.substr(3, 1).c_str()));
    Piece *piece = board->get_piece_at(start);
    if (piece) {
        board->move_piece(start, end);
    }
    // if (will_remain_in_check_after(start, end)) {
    //     std::cout << "Ai ramas in sah ba\n";
    // }
    if (board->is_piece_at(Position('a', 1))) {
        std::cout << board->get_piece_at(Position('a', 5))->type << "\n";
    }
}

bool Game::knight_check(Position king_pos, int col_offset, int row_offset) {
    Piece *encountered_piece;
    Color frendly_color = board->get_piece_at(king_pos)->color;
    if ((col_offset > 0 && king_pos.col + col_offset <= 'h') || (col_offset < 0 && king_pos.col + col_offset >= 'a')) {
        if ((row_offset > 0 && king_pos.row + row_offset <= 8) || (row_offset < 0 && king_pos.row + row_offset >= 1)) {
            encountered_piece = board->get_piece_at(Position(king_pos.col+col_offset, king_pos.row+row_offset));
            if (encountered_piece && encountered_piece->color != frendly_color && encountered_piece->type == Type::N) {
                return true;
            }
        }
    }
    return false;
}
