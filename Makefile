build: pieces.o board.o game.o engine.o
	g++ -g -Wall -Wextra -o main main.cpp pieces.o board.o game.o engine.o
	rm *.o

pieces.o:
	g++ -g -Wall -Wextra -c ./implementation/pieces.cpp

board.o:
	g++ -g -Wall -Wextra -c ./implementation/board.cpp

game.o:
	g++ -g -Wall -Wextra -c ./implementation/game.cpp

engine.o:
	g++ -g -Wall -Wextra -c ./implementation/engine.cpp

run: build
	./main

debug: build
	valgrind --leak-check=full --track-origins=yes ./main

clean:
	rm main