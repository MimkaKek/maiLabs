CC=g++
CFLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c -g -pg
OFILES=TPatriciaTrieItem.o TPatriciaTrie.o main.o

all: main

clear:
	rm *.o

main: $(OFILES)
	$(CC) $(OFILES) -g -pg -o main
TPatriciaTrieItem.o: TPatriciaTrieItem.h
	$(CC) $(CFLAGS) TPatriciaTrieItem.cpp
TPatriciaTrie.o: TPatriciaTrie.h
	$(CC) $(CFLAGS) TPatriciaTrie.cpp
main.o:
	$(CC) $(CFLAGS) main.cpp
