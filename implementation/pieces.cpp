#include "../skeleton/pieces.h"

std::ostream& operator<<(std::ostream& os, Type& type) {
    switch (type) {
    case K:
        os << "King";
        break;
    case Q:
        os << "Queen";
        break;
    case R:
        os << "Rook";
        break;
    case B:
        os << "Bishop";
        break;
    case N:
        os << "Knight";
        break;
    case P:
        os << "Pawn";
        break;
    }
    return os;
}

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
Piece::Piece(Type t, Color c, Position p, int pts) : type(t), color(c), pos(p), points(pts) {}

King::King(Color c, Position pos, int pts)      : Piece(Type::K, c, pos, pts)   {}
Queen::Queen(Color c, Position pos, int pts)    : Piece(Type::Q, c, pos, pts)   {}
Rook::Rook(Color c, Position pos, int pts)      : Piece(Type::R, c, pos, pts)   {}
Bishop::Bishop(Color c, Position pos, int pts)  : Piece(Type::B, c, pos, pts)   {}
Knight::Knight(Color c, Position pos, int pts)  : Piece(Type::N, c, pos, pts)   {}
Pawn::Pawn(Color c, Position pos, int pts)      : Piece(Type::P, c, pos, pts)   {}
