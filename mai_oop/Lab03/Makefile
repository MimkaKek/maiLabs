CC=g++
CFLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c

all: main.exe
	rm *.o
main.exe: main.o Rhombus.o Hexagon.o Pentagon.o TArray.o
	$(CC) Rhombus.o Hexagon.o Pentagon.o TArray.o main.o -o main.exe
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp
Hexagon.o: Hexagon.cpp
	$(CC) $(CFLAGS) Hexagon.cpp
Pentagon.o: Pentagon.cpp
	$(CC) $(CFLAGS) Pentagon.cpp
Rhombus.o: Rhombus.cpp
	$(CC) $(CFLAGS) Rhombus.cpp
TArray.o: TArray.cpp
	$(CC) $(CFLAGS) TArray.cpp

