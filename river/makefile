main:  main.o river.o
	g++ -g main.o river.o -o main

main.o: main.cpp river.h
	g++ -Wall -g -c main.cpp

river.o: river.cpp river.h
	g++ -Wall -g -c river.cpp


clean:
	rm -f *.o
