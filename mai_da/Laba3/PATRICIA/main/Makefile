CC=g++
OFLAGS=-ldmalloc -DDMALLOC -DDMALLOC_FUNC_CHECK
CFLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c
OFILES=TPatriciaTrieItem.o TPatriciaTrie.o main.o

all: main

clear:
	rm *.o *.bin

main: $(OFILES)
	$(CC) $(OFILES) -o main
TPatriciaTrieItem.o: TPatriciaTrieItem.h
	$(CC) $(CFLAGS) TPatriciaTrieItem.cpp
TPatriciaTrie.o: TPatriciaTrie.h
	$(CC) $(CFLAGS) TPatriciaTrie.cpp
main.o:
	$(CC) $(CFLAGS) main.cpp
