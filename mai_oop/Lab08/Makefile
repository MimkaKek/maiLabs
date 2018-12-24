CC=g++
CFLAGS=-pedantic -Wall -Werror -pthread -Wno-sign-compare -Wno-long-long -lm -c -g
CFILES=main.o Rhombus.o Hexagon.o Pentagon.o TArray.o TArrayItem.o TQueue.o TQueueItem.o TQueueSpec.o TQueueItemSpec.o TAllocationBlock.o

all: main.exe
main.exe: $(CFILES)
	$(CC) $(CFILES) -pthread -g -o main.exe
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp
Hexagon.o: Hexagon.cpp
	$(CC) $(CFLAGS) Hexagon.cpp
Pentagon.o: Pentagon.cpp
	$(CC) $(CFLAGS) Pentagon.cpp
Rhombus.o: Rhombus.cpp
	$(CC) $(CFLAGS) Rhombus.cpp
TArray.o: TArray.cpp
	$(CC) $(CFLAGS) TArray.cpp
TArrayItem.o: TArrayItem.cpp
	$(CC) $(CFLAGS) TArrayItem.cpp
TQueueSpec.o: TQueueSpec.cpp
	$(CC) $(CFLAGS) TQueueSpec.cpp
TQueueItemSpec.o: TQueueItemSpec.cpp
	$(CC) $(CFLAGS) TQueueItemSpec.cpp
TQueue.o: TQueue.cpp
	$(CC) $(CFLAGS) TQueue.cpp
TQueueItem.o: TQueueItem.cpp
	$(CC) $(CFLAGS) TQueueItem.cpp
TAllocationBlock.o: TAllocationBlock.cpp
	$(CC) $(CFLAGS) TAllocationBlock.cpp

