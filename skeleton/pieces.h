#pragma once
#include <assert.h>
#include <iostream>

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

    Position(char col, int row);
    
    bool operator==(const Position& other);
    bool operator!=(const Position& other);

    friend std::ostream& operator<<(std::ostream& out, const Position& p);
};

class Piece {
 public:
    Type type;
    Color color;
    Position pos;
    float points;

    Piece(Type t, Color c, Position p, float pts);
    
    void move_to(Position target);
};

class King : public Piece {
 public:
    King(Color c, Position pos);
};

class Queen : public Piece {
 public:
    Queen(Color c, Position pos);
};

class Rook : public Piece {
 public:
    Rook(Color c, Position pos);
};

class Bishop : public Piece {
 public:
    Bishop(Color c, Position pos);
};

class Knight : public Piece {
 public:
    Knight(Color c, Position pos);
};

class Pawn : public Piece {
 public:
    Pawn(Color c, Position pos);
};
