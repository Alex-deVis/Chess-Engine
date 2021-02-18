#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "pieces.h"

#define SIZE        8

struct EnumClassHash {
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

class Board {
 private:
    std::vector<std::vector<Piece*>> *board;
    std::unordered_map<Type, std::vector<Piece*>, EnumClassHash> white_pieces;
    std::unordered_map<Type, std::vector<Piece*>, EnumClassHash> black_pieces;
 
 public:
    Board();
    ~Board();

    void add_piece(Piece *piece);
    void add_piece(Piece *piece, Position position);
    bool is_piece_at(Position position);
    Piece* get_piece_at(Position position);

    void print_board();
};

/*
    Capture:    LSB set
    Castle:     second LSB set
    Promotion:  third LSB set
*/

class Configuration {
 public:
    std::string move_string;
    Position start, end;
    int value;

    Configuration(std::string move_string, bool capture, bool promotion, bool castle);
};
