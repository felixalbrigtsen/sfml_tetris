LINKER_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

main: main.cpp
	g++ -c main.cpp

game: tetris.cpp tetris.hpp
	g++ -c tetris.cpp -o game.o

out: main game
	g++ main.o game.o -o out $(LINKER_FLAGS)

clean:
	rm main.o game.o out
