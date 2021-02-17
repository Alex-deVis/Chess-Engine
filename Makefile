build:
	g++ -Wall -Wextra -o main main.cpp

run: build
	./main

clean:
	rm main