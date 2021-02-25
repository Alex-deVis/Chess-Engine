#pragma once
#include <vector>
#include "board.h"
#define INFINITY    1000

class Game {
public:
   Game();
   Game(Game *game);
   ~Game();

   void move(std::string move_string, bool forced = true);
   void undo_move();
   bool valid_move(Position start, Position end, bool suppress_msg = false);
   void print_board();
   void turn(Color color);
   double evaluate();
   bool ended();
   void which_turn();
   std::vector<std::string> possible_moves_for(Color color);
   bool is_piece_at(Position pos);
   std::string last_played_move();

private:
   Board *board;
   std::vector<Transition> *played_moves;
   bool white_to_move;
   int taken_white, taken_black;

   bool are_possible_moves_for(Color color);

   // Check moving patterns
   bool can_reach(King *p, Position end);
   bool can_reach(Queen *p, Position end);
   bool can_reach(Rook *p, Position end);
   bool can_reach(Bishop *p, Position end);
   bool can_reach(Knight *p, Position end);
   bool can_reach(Pawn *p, Position end);

   // Avoid check-related illegal moves
   bool will_remain_in_check_after(Position start, Position end);
   bool can_color_reach(Color color, Position target);

   // Helpers
   bool has_moved(Piece *p);
   bool is_knight_at(Position pos, int col_off, int row_off, Color color);
   Transition generate_transition(std::string move_string);

   // Engine
   std::vector<std::string> possible_moves_for(King *p);
   std::vector<std::string> possible_moves_for(Queen *p);
   std::vector<std::string> possible_moves_for(Rook *p);
   std::vector<std::string> possible_moves_for(Bishop *p);
   std::vector<std::string> possible_moves_for(Knight *p);
   std::vector<std::string> possible_moves_for(Pawn *p);
};
