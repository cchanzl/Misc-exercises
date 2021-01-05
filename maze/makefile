main:  main.o maze.o
	g++ -g main.o maze.o -o main

main.o: main.cpp maze.h
	g++ -Wall -g -c main.cpp

maze.o: maze.cpp maze.h
	g++ -Wall -g -c maze.cpp


clean:
	rm -f *.o
