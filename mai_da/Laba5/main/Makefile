#======================================
# Параметры сборки
# TARGET - название исполняемого файла
#======================================
TARGET  = main
#======================================
# Параметры компиляции
# СС - компилятор
# CFLAGS - флаги для компиляции
#======================================
CC      =g++
CFLAGS  =-Wall -Wextra -Werror -pedantic -Wno-sign-compare -Wno-long-long -lm -O3 -c
#======================================
# Тут подхватываются файлы .cpp
#======================================
SRC    := main.cpp
#======================================

OBJS   := main.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(DEBUG) $^ -o $@
	
%.o: %.cpp 
	$(CC) $(CFLAGS) $(DEBUG) $<

clean:
	rm *.o $(TARGET)
