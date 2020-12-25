doublets:  main.o doublets.o
	g++ -g main.o doublets.o -o doublets

main.o: main.cpp doublets.h
	g++ -Wall -g -c main.cpp

doublets.o: doublets.cpp dictionary.h doublets.h
	g++ -Wall -g -c doublets.cpp


clean:
	rm -f *.o
