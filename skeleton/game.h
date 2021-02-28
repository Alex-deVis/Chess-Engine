#pragma once
#include <vector>
#include "board.h"

class Game {
public:
   Game();
   Game(Game *game);
   ~Game();

   // User interaction
   bool move(std::string move_string, bool forced = true);
   void turn(Color color);
   void print_board();
   bool ended(bool suppress_msg = true);

   // Engine
   std::vector<std::string> possible_moves_for(Color color);
   void undo_move();
   
   // Setter
   void clear_move_queue();

   // Getters
   int get_taken(Color color);
   int who_won();
   bool in_the_endgame();
   Piece *get_piece_at(Position pos);

private:
   Board *board;
   std::vector<Transition> *played_moves;
   bool white_to_move;
   int taken_white, taken_black;

   // Move legality
   bool valid_move(Position start, Position end, bool suppress_msg = false);

   // Check moving patterns
   bool can_reach(King *p, Position end);
   bool can_reach(Queen *p, Position end);
   bool can_reach(Rook *p, Position end);
   bool can_reach(Bishop *p, Position end);
   bool can_reach(Knight *p, Position end);
   bool can_reach(Pawn *p, Position end);

   // Avoid check-related illegal moves
   bool will_remain_in_check_after(Position start, Position end);

   // Helpers
   bool has_moved(Piece *p);
   bool is_knight_at(Position pos, int col_off, int row_off, Color color);
   Transition generate_transition(std::string move_string);
   bool can_color_reach(Color color, Position target);

   std::vector<std::string> possible_moves_for(King *p);
   std::vector<std::string> possible_moves_for(Queen *p);
   std::vector<std::string> possible_moves_for(Rook *p);
   std::vector<std::string> possible_moves_for(Bishop *p);
   std::vector<std::string> possible_moves_for(Knight *p);
   std::vector<std::string> possible_moves_for(Pawn *p);

   // Engine
   bool are_possible_moves_for(Color color);
};
