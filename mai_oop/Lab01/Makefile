all: main.exe
	rm *.o
main.exe: main.o Hexagon.o Pentagon.o Rhombus.o TStackItem.o TStack.o
	g++ -Wall main.o Hexagon.o Pentagon.o Rhombus.o -o main.exe
main.o: main.cpp
	g++ -Wall -c main.cpp
Hexagon.o: Hexagon.cpp
	g++ -Wall -c Hexagon.cpp
Pentagon.o: Pentagon.cpp
	g++ -Wall -c Pentagon.cpp
Rhombus.o: Rhombus.cpp
	g++ -Wall -c Rhombus.cpp
TStackItem.o: TStackItem.cpp
	g++ -Wall -c TStackItem.cpp
TStack.o: TStack.cpp
	g++ -Wall -c TStack.cpp

