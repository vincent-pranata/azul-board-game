.default: all

all: azul

clean:
	rm -rf azul *.o *.dSYM

azul: Tile.o Node.o LinkedList.o Azul.o Menu.o GameEngine.o Player.o Board.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c $^

