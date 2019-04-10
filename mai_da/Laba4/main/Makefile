CC=g++
CFLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -g -c
OFILES=TAhoCorasickNode.o TAhoCorasick.o main.o

all: main

clear:
	rm *.o
main: $(OFILES)
	$(CC) $(OFILES) -g -o main
TPatriciaTrieItem.o: TAhoCorasickNode.h
	$(CC) $(CFLAGS) TAhoCorasickNode.cpp
TPatriciaTrie.o: TAhoCorasick.h
	$(CC) $(CFLAGS) TAhoCorasick.cpp
main.o:
	$(CC) $(CFLAGS) main.cpp
