main:  main.o scrabble.o
	g++ -g main.o scrabble.o -o main

main.o: main.cpp scrabble.h
	g++ -Wall -g -c main.cpp

scrabble.o: scrabble.cpp scrabble.h
	g++ -Wall -g -c scrabble.cpp


clean:
	rm -f *.o
