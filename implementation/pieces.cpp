#include "../skeleton/pieces.h"

Position::Position(char col, int row) : col(col), row(row) {
    if (!(col >= 'a' && col <= 'h' && row >= 1 && row <= 8)) {
        std::cout << col << row << "\n";
    }
    assert(col >= 'a' && col <= 'h' && row >= 1 && row <= 8);
}

bool Position::operator==(const Position& other) {
    return this->col == other.col && this->row == other.row;
}

bool Position::operator!=(const Position& other) {
    return (this->col  != other.col || this->row != other.row);
}

std::ostream& operator<<(std::ostream& out, const Position& p) {
    out << p.col << p.row;
    return out;
}

// Piece
Piece::Piece(Type t, Color c, Position p, float pts) : type(t), color(c), pos(p), points(pts) {}
void Piece::move_to(Position target) {
    std::cout << "Attempting to move " << this->type << " from " << this->pos << " to " << target << "\n";
    this->pos = target;
}

King::King(Color c, Position pos) : Piece(Type::K, c, pos, KING_POINTS)         {}
Queen::Queen(Color c, Position pos) : Piece(Type::Q, c, pos, QUEEN_POINTS)      {}
Rook::Rook(Color c, Position pos) : Piece(Type::R, c, pos, ROOK_POINTS)         {}
Bishop::Bishop(Color c, Position pos) : Piece(Type::B, c, pos, BISHOP_POINTS)   {}
Knight::Knight(Color c, Position pos) : Piece(Type::N, c, pos, KNIGHT_POINTS)   {}
Pawn::Pawn(Color c, Position pos) : Piece(Type::P, c, pos, PAWN_POINTS)         {}
