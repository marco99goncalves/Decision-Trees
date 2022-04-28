output: Main.o Util.o
	g++ -Ofast Main.o Util.o -Wall -o out

Main.o: Main.cpp
	g++ -Ofast -c Main.cpp

Util.o: Util.cpp Util.h
	g++ -Ofast -c Util.cpp

clean:
	rm *.o out