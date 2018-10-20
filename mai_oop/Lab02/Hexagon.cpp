#include "Hexagon.h"
#include <iostream>
#include <cmath>

Hexagon::Hexagon() : Hexagon(0) {
}

Hexagon::Hexagon(size_t i) : side_a(i) {
    std::cout << "Hexagon created: " << side_a << std::endl;
}

Hexagon::Hexagon(std::istream &is) {
    is >> side_a;
    std::cout << "Hexagon created!" << std::endl;
}

Hexagon::Hexagon(const Hexagon& orig) {
    std::cout << "Hexagon copy created" << std::endl;
    side_a = orig.side_a;
}

double Hexagon::Square() {
    double tmp = ((3*sqrt(3))/2) * pow(side_a, 2);
    std::cout << "Square = " << tmp << std::endl;
    return tmp;
}

bool operator==(const Hexagon& left,const Hexagon& right) {
	if(left.side_a == right.side_a)
		return true;
	else
		return false;
}

void Hexagon::Print() {
    std::cout << "Hexagon: a = " << side_a << std::endl;
}

Hexagon::~Hexagon() {
    std::cout << "Hexagon deleted" << std::endl;
}


