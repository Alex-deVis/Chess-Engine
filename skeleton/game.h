#pragma once
#include <vector>
#include "board.h"

class Game {
public:
   Game();
   ~Game();

   void move(std::string move_string, bool forced = true);
   void undo_move();
   bool valid_move(Position start, Position end);
   void print_board();

private:
   Board *board;
   std::vector<Transition> *played_moves;
   bool white_to_move;

   // Check moving patterns
   bool can_reach(King *p, Position end);
   bool can_reach(Queen *p, Position end);
   bool can_reach(Rook *p, Position end);
   bool can_reach(Bishop *p, Position end);
   bool can_reach(Knight *p, Position end);
   bool can_reach(Pawn *p, Position end);

   // Helpers
   bool has_moved(Piece *p);
   Transition generate_transition(std::string move_string);
   bool will_remain_in_check_after(Position start, Position end);
   bool can_color_reach(Color color, Position target);
   bool can_knight_reach(Position king_pos, int col_offset, int row_offset, Color frendly_color);
   bool is_knight_at(Position pos, int col_off, int row_off, Color color);
};