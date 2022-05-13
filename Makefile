output: Main.o Util.o Data.o Node.o
	g++ -Ofast Main.o Util.o Data.o Node.o -Wall -o out

Main.o: Main.cpp
	g++ -Ofast -c Main.cpp

Util.o: Util.cpp Util.h
	g++ -Ofast -c Util.cpp

Data.o: Data.cpp Data.h
	g++ -Ofast -c Data.cpp

Node.o: Node.cpp Node.h
	g++ -Ofast -c Node.cpp

clean:
	rm *.o out