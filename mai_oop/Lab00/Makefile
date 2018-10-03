all: main.exe
	rm *.o
main.exe: main.o Hexagon.o Pentagon.o Rhombus.o
	g++ -Wall main.o Hexagon.o Pentagon.o Rhombus.o -o main.exe
main.o: main.cpp
	g++ -Wall -c main.cpp
Hexagon.o: Hexagon.cpp
	g++ -Wall -c Hexagon.cpp
Pentagon.o: Pentagon.cpp
	g++ -Wall -c Pentagon.cpp
Rhombus.o: Rhombus.cpp
	g++ -Wall -c Rhombus.cpp
