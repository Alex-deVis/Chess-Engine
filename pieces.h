#pragma once
#include <assert.h>

#define WHITE_PIECES_ROW        1
#define WHITE_PAWNS_ROW         2
#define BLACK_PIECES_ROW        8
#define BLACK_PAWNS_ROW         7

#define KING_POINTS             100
#define QUEEN_POINTS            9
#define ROOK_POINTS             5
#define BISHOP_POINTS           3.25
#define KNIGHT_POINTS           3.15
#define PAWN_POINTS             1

enum Type {K, Q, R, B, N, P};
enum Color {WHITE, BLACK};

class Position {
 public:
    char col;
    int row;

    Position(char col, int row) : col(col), row(row) {
        if (!(col >= 'a' && col <= 'h' && row >= 1 && row <= 8)) {
            std::cout << col << row << "\n";
        }
        assert(col >= 'a' && col <= 'h' && row >= 1 && row <= 8);
    }
    friend std::ostream& operator<<(std::ostream& out, const Position& p);
};

std::ostream& operator<<(std::ostream& out, const Position& p) {
    out << p.col << p.row;
    return out;
}

class Piece {
 public:
    Type type;
    Color color;
    Position pos;
    float points;

    Piece(Type t, Color c, Position p, float pts) : type(t), color(c), pos(p), points(pts) {}
    
    void move_to(Position target) {
        std::cout << "Attempting to move " << this->type << " from " << this->pos << " to " << target << "\n";
        this->pos = target;
    }
};

class King : public Piece {
 public:
    King(Color c, Position pos) : Piece(Type::K, c, pos, KING_POINTS) {}
};

class Queen : public Piece {
 public:
    Queen(Color c, Position pos) : Piece(Type::Q, c, pos, QUEEN_POINTS) {}
};

class Rook : public Piece {
 public:
    Rook(Color c, Position pos) : Piece(Type::R, c, pos, ROOK_POINTS) {}
};

class Bishop : public Piece {
 public:
    Bishop(Color c, Position pos) : Piece(Type::B, c, pos, BISHOP_POINTS) {}
};

class Knight : public Piece {
 public:
    Knight(Color c, Position pos) : Piece(Type::N, c, pos, KNIGHT_POINTS) {}
};

class Pawn : public Piece {
 public:
    Pawn(Color c, Position pos) : Piece(Type::P, c, pos, PAWN_POINTS) {}
};
