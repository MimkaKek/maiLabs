//intdiv.cpp
/*
(/ 3 2)
(+(/ 3 2)(/ 3 2))
*/
#include "mlisp.h"
int main(){
 display(3/2);newline();
 display(3/2 + 3/2);newline();
 display(double(3)/2 + double(3)/2);newline();
std::cin.get();
 return 0;
}
