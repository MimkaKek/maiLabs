all: main.exe
	rm *.o
main.exe: main.o Rhombus.o Hexagon.o Pentagon.o TArray.o
	g++ -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -g main.o TArray.o Pentagon.o Hexagon.o Rhombus.o -o main.exe
main.o: main.cpp
	g++ -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c -g main.cpp
Hexagon.o: Hexagon.cpp
	g++ -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c -g Hexagon.cpp
Pentagon.o: Pentagon.cpp
	g++ -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c -g Pentagon.cpp
Rhombus.o: Rhombus.cpp
	g++ -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c -g Rhombus.cpp
TArray.o: TArray.cpp
	g++ -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c -g TArray.cpp

