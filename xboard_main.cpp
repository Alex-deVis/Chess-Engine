#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <limits>
#include <vector>
#include "skeleton/game.h"

// Pentru etapa 1 am implementat logica mutarilor si comunicarea cu xboard in main
int main() {
	Game* chess = new Game();
	// Piesa pe care o mutam pentru etapa 1
	std::string cmd, aux, mov = "a1a1";
    bool white_to_move = false;
    bool forced = false;

	// Dezactivarea buffer-elor de input si output
	std::cout.setf(std::ios::unitbuf);
	std::cin.setf(std::ios::unitbuf);
	
	while (true) {
		// Citirea comenzii date de xboard
		getline(std::cin, cmd, '\n');

		std::istringstream iss(cmd);
    	std::string token, aux;
		std::getline(iss, token, ' ');

        if (token == "quit") {
			break;
		}

		if (token == "protover") {  // Dupa comanda "protover" precizam optiunile pentru xboard
			std::cout << "feature ping=1 setboard=1 colors=1 usermove=1 sigint=0 myname=\"BombardiSah\"\n" << "feature done=1\n";
		} else if (token == "white") {  // Engine plays with white
            white_to_move = true;
		} else if (token == "force") {  // Engine will receive a forced move
			forced = true;
		} else if (token == "new") {  // New game
			delete chess;
			chess = new Game();
		} else if (token == "ping") {  // Communication test
			std::getline(iss, aux, ' ');
			std::cout << "pong " << aux << "\n";
		} else if (token == "time" || token == "otim" || token == "result" || token == "level" || token == "accepted" ||
                token == "easy" || token == "hard" || token == "post" || token == "random" || token == "resign" ||
                token == "xboard" || token == "black" || token == "go" || token == "computer") {
            // Commands which do not require management
			continue;
		}

		if (token == "usermove") {
            // Recieve oponent`s move
			std::getline(iss, aux, ' ');
			// Facem mutarea in board-ul intern
			chess->move(aux);
		}

		// La comanda quit iese din program
		

		// Mutarea pentru alb
		if (white_to_move) {
			std::cout << "move " << "a2a4" << '\n';
		} else {
			std::cout << "move " << "a7a5" << '\n';
		}
	}

	delete chess;
	return 0;
}