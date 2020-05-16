WAY=../Lib/
CC=g++
CFLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c
OFILES=main.o matrix.o interpolation.o

all: main

clean:
	rm *.o main

main: $(OFILES)
	$(CC) $(OFILES) -o main
main.o:
	$(CC) $(CFLAGS) main.cpp
matrix.o:
	$(CC) $(CFLAGS) $(WAY)matrix.cpp
interpolation.o:
	$(CC) $(CFLAGS) $(WAY)interpolation.cpp
