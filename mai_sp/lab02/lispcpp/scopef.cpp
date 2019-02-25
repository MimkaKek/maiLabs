//scopef.cpp
/*
(define(f f)
  (display "f=")(display f)(newline)
  (let(
       (f 5)
      )
      f
  )
)
(f 2)
*/
#include "mlisp.h"
double f(double f){
 display("f=");display(f);newline();
 {//let
  double f(5);
  return f;
 }//let
}
int main(){
 display(f(2));newline();
std::cin.get();
 return 0;
}
