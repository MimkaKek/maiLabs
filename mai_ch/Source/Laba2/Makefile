TARGET  = main
ILIB    = ../Lib/
LSRC   := matrix.cpp aproxcalc.cpp functions.cpp

CC = g++
CFLAGS = -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c

LSRC   := $(LSRC: %=$(ILIB)%)

SRC    := $(wildcard *.cpp) $(LSRC)

OBJS   := $(SRC:$(ILIB)%=%)
OBJS   := $(OBJS:.cpp=.o)

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(OBJS)
ifdef debug
	$(CC) -g $^ -o $@
else
	$(CC) $^ -o $@
endif
	

%.o: %.cpp
ifdef debug
	$(CC) $(CFLAGS) -g $<
else
	$(CC) $(CFLAGS) $<
endif
	

%.o: $(ILIB)%.cpp
ifdef debug
	$(CC) $(CFLAGS) -g $<
else
	$(CC) $(CFLAGS) $<
endif

clean:
	rm *.o $(TARGET)
