CC=g++
CFLAGS=-pedantic -Wall -Werror -pthread -Wno-sign-compare -Wno-long-long -lm -c -g
CFILES=main.o Rhombus.o TArray.o

all: main.exe
main.exe: $(CFILES)
	$(CC) $(CFILES) -pthread -g -o main.exe
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp
Rhombus.o: Rhombus.cpp
	$(CC) $(CFLAGS) Rhombus.cpp
TArray.o: TArray.cpp
	$(CC) $(CFLAGS) TArray.cpp
