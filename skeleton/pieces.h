#pragma once
#include <assert.h>
#include <iostream>

#define WHITE_PIECES_ROW        1
#define WHITE_PAWNS_ROW         2
#define BLACK_PIECES_ROW        8
#define BLACK_PAWNS_ROW         7

#define KING_POINTS             20000
#define QUEEN_POINTS            900
#define ROOK_POINTS             500
#define BISHOP_POINTS           330
#define KNIGHT_POINTS           320
#define PAWN_POINTS             100

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
    int points;

    Piece(Type t, Color c, Position p, int pts);
};

class King : public Piece {
 public:
    King(Color c, Position pos, int pts = KING_POINTS);
};

class Queen : public Piece {
 public:
    Queen(Color c, Position pos, int pts = QUEEN_POINTS);
};

class Rook : public Piece {
 public:
    Rook(Color c, Position pos, int pts = ROOK_POINTS);
};

class Bishop : public Piece {
 public:
    Bishop(Color c, Position pos, int pts = BISHOP_POINTS);
};

class Knight : public Piece {
 public:
    Knight(Color c, Position pos, int pts = KNIGHT_POINTS);
};

class Pawn : public Piece {
 public:
    Pawn(Color c, Position pos, int pts = PAWN_POINTS);
};
