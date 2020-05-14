TARGET=main
CFLAGS=-Wall -Wextra -Werror -pedantic -Wno-sign-compare -Wno-long-long -lm -O3 -std=c++11
DEBUG=-ggdb -g3

all: $(TARGET)

$(TARGET): main.o
	g++ main.o -o main $(CFLAGS)
main.o:
	g++ $(CFLAGS) -c main.cpp
sffxtr.o:
	g++ $(CFLAGS) -c sffxtr.cpp
sffxtrn.o:
	g++ $(CFLAGS) -c sffxtrn.cpp
	
debug: main_dbg.o
	g++ main_dbg.o -o $(TARGET)_dbg
main_dbg.o: main.cpp
	g++ -c main.cpp -o main_dbg.o $(DEBUG)
	
clean:
	rm -f *.o $(TARGET)
