CC=g++
CFLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c -g

all: test.exe
	rm *.o
test.exe: test.o
	$(CC) test.o -g -o test.exe
test.o: test.cpp
	$(CC) $(CFLAGS) test.cpp
