main:  main.o piglatin.o
	g++ -g main.o latin.o -o main

main.o: main.cpp piglatin.h
	g++ -Wall -g -c main.cpp

piglatin.o: piglatin.cpp piglatin.h
	g++ -Wall -g -c piglatin.cpp


clean:
	rm -f *.o
