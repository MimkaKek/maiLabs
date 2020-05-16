WAY=../Lib/
CC=g++
CFLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c
OFILES=main.o matrix.o aproxcalc.o functions.o

all: main

clean:
	rm *.o main

main: $(OFILES)
	$(CC) $(OFILES) -o main
main.o:
	$(CC) $(CFLAGS) main.cpp
matrix.o:
	$(CC) $(CFLAGS) $(WAY)matrix.cpp
aproxcalc.o:
	$(CC) $(CFLAGS) $(WAY)aproxcalc.cpp
functions.o:
	$(CC) $(CFLAGS) $(WAY)functions.cpp
