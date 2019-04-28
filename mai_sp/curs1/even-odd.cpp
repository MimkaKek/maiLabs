/*  GVV2019   */
#include "mlisp.h"
double even__bits(double n);
double odd__bits(double n);
double report__results(double n);
double dd = 3;
double mm = 3;
double yyyy = 1999;
//________________ 
double even__bits(double n) {
return (n == 0) ? 1 : (remainder(n, 2) == 0) ? even__bits(quotient(n, 2)) : odd__bits(quotient(n, 2));
}

double odd__bits(double n) {
return (n == 0) ? 0 : (remainder(n, 2) == 0) ? odd__bits(quotient(n, 2)) : true ? even__bits(quotient(n, 2)) : _infinity;
}

double report__results(double n) {
display("Happy birthday to you_E\n\t");
display(n);
display(" (decimal)\n\t");
display__bin(n);
display("(reversed binary)\n");
display("\teven_Q\t");
display(( (even__bits(n) == 1) ? "yes" : "no"));
newline();
display("\todd_Q\t");
display(( (odd__bits(n) == 1) ? "yes" : "no"));
newline();
return 0;
}



int main(){
 display(report__results(((dd * 1000000)(mm * 10000) + yyyy))); newline();
 std::cin.get();
 return 0;
}

