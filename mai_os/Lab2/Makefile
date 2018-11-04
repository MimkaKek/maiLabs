CC=gcc
CFLAGS=-pthread

all: start
	rm *.o
start: main.o
	$(CC) $(CFLAGS) main.o -o start.exec
main.o:
	$(CC) $(CFLAGS) -c main.c
