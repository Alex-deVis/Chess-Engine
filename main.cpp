#include <iostream>
#include <sstream>
#include "skeleton/engine.h"

using namespace std;

int main() {
	Engine *Devis_engine = new Engine(4);
    Devis_engine->set_color(Color::BLACK);
    bool forced = false;
	std::string cmd, token, my_move, enemy_move;

	std::cout.setf(std::ios::unitbuf);
	std::cin.setf(std::ios::unitbuf);
	
	while (true) {
		getline(std::cin, cmd, '\n');
		std::istringstream input(cmd);
		
        input >> token;

        if (token == "quit") {
			break;
		} else if (token == "protover") {
			std::cout << "feature ping=1 setboard=1 colors=1 usermove=1 sigint=0 myname=\"BombardiSah\"\n" << "feature done=1\n";
        } else if (token == "new") {
			Devis_engine->new_game();
        } else if (token == "ping") {
            std::string aux;
            input >> aux;
            std::cout << "pong " << aux << "\n";
        } else if (token == "usermove") {
            input >> enemy_move;
            Devis_engine->play_move(enemy_move, forced);
            my_move = Devis_engine->generate_move();
            Devis_engine->play_move(my_move, forced);
            std::cout << "move " << my_move << "\n";
        } else if (token == "white") {
            Devis_engine->set_color(Color::WHITE);   
        } else if (token == "black") {
            Devis_engine->set_color(Color::BLACK);
        } else if (token == "forced") {
            forced = !forced;
        }
	}
	delete Devis_engine;
	
	return 0;
}
