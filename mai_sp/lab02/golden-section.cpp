#include "mlisp.h"

double fun(double);
double golden__section__search(double, double);
double golden__start(double, double);
double __GVV__try(double, double, double, double, double, double);

double a = -1;
double b = 1;
double eps = 0.00001;
double mphi = 0;
double xmin = 0;

double fun(double x) {
	x = x - double(101) / double(102) / e;
	return expt(sin(2*exp(-x) - 4), 3);
}

double golden__section__search(double a, double b) {
	{
		xmin = (a < b) ? golden__start(a, b) : golden__start(b, a);
		newline();
		return xmin;
	}
}

double golden__start(double a, double b) {
	mphi = 0.5  * (3 - sqrt(5));
	{
		double xa = a + (mphi * (b - a));
		double xb = b - (mphi * (b - a));
		return __GVV__try(a, b, xa, fun(xa), xb, fun(xb));
	}
}

double __GVV__try(double a, double b, double xa, double ya, double xb, double yb) {
	return (abs(a - b) < eps) ? (a + b)*0.5 
		   : (true) ? (display("+") , (ya < yb) ? (b = xb, xb = xa, yb = ya, xa = a + mphi*(b - a), __GVV__try(a, b, xa, fun(xa), xb, yb))
		   :                                     (a = xa, xa = xb, ya = yb, xb = b - mphi*(b - a), __GVV__try(a, b, xa, ya, xb, fun(xb)))) : (_infinity);
}

int main() {
    xmin = golden__section__search(a, b);
    display("interval=\t[");
    display(a);
    display(" , ");
    display (b);
    display("]\n");
    display("xmin=\t\t");
    display(xmin);
    newline();
    display("f(xmin)=\t");
    display(fun(xmin));
	newline();
	std::cin.get();
	return 0;
}
