#include "Rhombus.h"
#include <iostream>
#include <cmath>

Rhombus::Rhombus() : Rhombus(0, 0) {
}

Rhombus::Rhombus(size_t i, size_t j) : side_d1(i), side_d2(j) {
    std::cout << "Rhombus created: " << side_d1 << ", " << side_d2 << std::endl;
}

Rhombus::Rhombus(std::istream &is) {
    is >> side_d2;
    is >> side_d1;
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
    std::cout << "d1 = " << side_d1 << ", d2 = " << side_d2 << std::endl;

}

Rhombus::~Rhombus() {
    std::cout << "Rhombus deleted" << std::endl;
}


