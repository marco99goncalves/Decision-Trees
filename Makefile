output: Main.o Util.o Data.o
	g++ -Ofast Main.o Util.o Data.o -Wall -o out

Main.o: Main.cpp
	g++ -Ofast -c Main.cpp

Util.o: Util.cpp Util.h
	g++ -Ofast -c Util.cpp

Data.o: Data.cpp Data.h
	g++ -Ofast -c Data.cpp

clean:
	rm *.o out