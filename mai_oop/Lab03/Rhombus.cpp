#include "Rhombus.h"
#include <iostream>
#include <cmath>

Rhombus::Rhombus() : Rhombus(0, 0) {
}

Rhombus::Rhombus(int i, int j) : side_d1(i), side_d2(j) {
    std::cout << "Rhombus created: " << side_d1 << ", " << side_d2 << std::endl;
}

Rhombus::Rhombus(std::istream &is) {
    do {
        is >> side_d1;
        is >> side_d2;
        if(side_d1 < 0 || side_d2 < 0) {
          std::cout << "Number is < 0. Enter another numbers: ";
        }
    } while(side_d1 < 0 || side_d2 < 0);
    std::cout << "Rhombus created!" << std::endl;
}

Rhombus::Rhombus(const Rhombus& orig) {
    std::cout << "Rhombus copy created" << std::endl;
    side_d1 = orig.side_d1;
    side_d2 = orig.side_d2;
}

double Rhombus::Square() {
    double tmp = (side_d1 * side_d2)/2;
    std::cout << "Square = " << tmp << std::endl;
    return tmp;
}

void Rhombus::Print() {
    std::cout << "Rhombus: d1 = " << side_d1 << ", d2 = " << side_d2 << std::endl;
}

Rhombus::~Rhombus() {
    std::cout << "Rhombus deleted" << std::endl;
}

std::ostream& operator<<(std::ostream& os,const Rhombus& rhombus) {
	os << "d1 = " << rhombus.side_d1 << "d2 = " << rhombus.side_d2 << std::endl;
	return os;
}

bool operator==(const Rhombus& left,const Rhombus& right) {
	if(left.side_d1 == right.side_d1 && left.side_d2 == right.side_d2)
		return true;
	else
		return false;
}


