CXX 		= g++
CPPFLAGS 	= -g -Wall -Wextra

build: pieces.o board.o game.o engine.o
	$(CXX) $(CPPFLAGS) -o devis_engine main.cpp pieces.o board.o game.o engine.o
	rm *.o

pieces.o:
	$(CXX) $(CPPFLAGS) -c ./implementation/pieces.cpp

board.o:
	$(CXX) $(CPPFLAGS) -c ./implementation/board.cpp

game.o:
	$(CXX) $(CPPFLAGS) -c ./implementation/game.cpp

engine.o:
	$(CXX) $(CPPFLAGS) -c ./implementation/engine.cpp

run: build
	./main

clean:
	rm devis_engine