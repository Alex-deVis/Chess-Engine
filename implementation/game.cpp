#include "../skeleton/game.h"
Game::Game() {
    this->board = new Board();
    this->played_moves = new std::stack<Configuration>();
}

Game::~Game() {
    delete this->board;
    delete this->played_moves;
}

bool Game::check_move(std::string move_string) {
    assert(("Move length must be equal to 4." && move_string.length() == 4));

    Position start = Position(move_string[0], atoi(move_string.substr(1, 1).c_str()));
    Position end = Position(move_string[2], atoi(move_string.substr(3, 1).c_str()));

    std::cout << start << " & " << end << "\n";
    return false;
}

void Game::print_board() {
    this->board->print_board();
}

bool Game::moving_linearly(Position start, Position end) {
    return (start.col == end.col || start.row == end.row);
}

bool Game::moving_diagonaly(Position start, Position end) {
    return abs(start.col - end.col) == abs(start.row - end.row);
}

