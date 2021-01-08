main:  main.o minesweeper.o
	g++ -g main.o minesweeper.o -o main

main.o: main.cpp minesweeper.h
	g++ -Wall -g -c main.cpp

minesweeper.o: minesweeper.cpp minesweeper.h
	g++ -Wall -g -c minesweeper.cpp


clean:
	rm -f *.o
