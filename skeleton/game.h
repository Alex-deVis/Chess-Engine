#pragma once
#include <vector>
#include "board.h"

class Game {
public:
   Game();
   ~Game();

   void move(std::string move_string, bool forced = true);
   bool valid_move(Position start, Position end);
   void print_board();

private:
   Board *board;
   std::vector<Transition> *played_moves;
   bool white_to_move;

   bool can_reach(King *p, Position end);
   bool can_reach(Queen *p, Position end);
   bool can_reach(Rook *p, Position end);
   bool can_reach(Bishop *p, Position end);
   bool can_reach(Knight *p, Position end);
   bool can_reach(Pawn *p, Position end);

   bool has_moved(Piece *p);

   bool will_remain_in_check_after(Position start, Position end);

   Transition generate_transition(std::string move_string);

   bool knight_check(Position king_pos, int col_offset, int row_offset);
   bool moving_linearly(Position start, Position end);
   bool moving_diagonaly(Position start, Position end);
};