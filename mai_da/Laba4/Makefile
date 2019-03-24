CC=g++
CFLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c
OFILES=TAhiKorasikNode.o TAhiKorasik.o main.o

all: main
	rm *.o

main: $(OFILES)
	$(CC) $(OFILES) -o main
TPatriciaTrieItem.o: TAhiKorasikNode.h
	$(CC) $(CFLAGS) TAhiKorasikNode.cpp
TPatriciaTrie.o: TAhiKorasik.h
	$(CC) $(CFLAGS) TAhiKorasik.cpp
main.o:
	$(CC) $(CFLAGS) main.cpp
