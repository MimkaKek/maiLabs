#include "Pentagon.h"
#include <iostream>
#include <cmath>

const double pi = 3.141592;
const double k = 0.688191;

Pentagon::Pentagon() : Pentagon(0) {
}

Pentagon::Pentagon(size_t i) : side_a(i) {
    std::cout << "Pentagon created: " << side_a << std::endl;
}

Pentagon::Pentagon(std::istream &is) {
    is >> side_a;
}

Pentagon::Pentagon(const Pentagon& orig) {
    std::cout << "Pentagon copy created" << std::endl;
    side_a = orig.side_a;
}

double Pentagon::Square() {
    double r = k * side_a;
    double tmp = 5 * pow(r, 2) * tan(pi/5);
    std::cout << "Square = " << tmp << std::endl;
    return tmp;
}

void Pentagon::Print() {
    std::cout << "a = " << side_a << std::endl;
}

Pentagon::~Pentagon() {
    std::cout << "Pentagon deleted" << std::endl;
}


