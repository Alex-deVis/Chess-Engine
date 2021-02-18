build: pieces.o board.o game.o
	g++ -Wall -Wextra -o main main.cpp pieces.o board.o game.o
	rm *.o

pieces.o:
	g++ -Wall -Wextra -c ./implementation/pieces.cpp

board.o:
	g++ -Wall -Wextra -c ./implementation/board.cpp

game.o:
	g++ -Wall -Wextra -c ./implementation/game.cpp

run: build
	./main

debug: build
	valgrind --leak-check=full --track-origins=yes ./main

clean:
	rm main
