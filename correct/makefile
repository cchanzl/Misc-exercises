main:  main.o correct.o
	g++ -g main.o correct.o -o main

main.o: main.cpp correct.h
	g++ -Wall -g -c main.cpp

correct.o: correct.cpp correct.h
	g++ -Wall -g -c correct.cpp


clean:
	rm -f *.o
