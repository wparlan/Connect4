all:
	g++ -c -std=c++11 ArcadeMain.cpp C4.cpp
	g++ ArcadeMain.o C4.o -o C4
	./C4
compile:
	g++ -c -std=c++11 ArcadeMain.cpp C4.cpp
	g++ ArcadeMain.o C4.o -o C4
run:
	./C4
