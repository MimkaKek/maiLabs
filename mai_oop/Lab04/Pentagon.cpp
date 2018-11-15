#include "Pentagon.h"
#include <iostream>
#include <cmath>

const double pi = 3.141592;
const double k = 0.688191;

Pentagon::Pentagon() : Pentagon(0) {
}

Pentagon::Pentagon(int i) : side_a(i) {
    std::cout << "Pentagon created: " << side_a << std::endl;
}

Pentagon::Pentagon(std::istream &is) {
    do {
        is >> side_a;
        if(side_a < 0) {
          std::cout << "Number is < 0. Enter another number: ";
        }
    } while(side_a < 0);
    std::cout << "Pentagon created!" << std::endl;
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

std::ostream& operator<<(std::ostream& os,const Pentagon& pentagon) {
  os << "a = " << pentagon.side_a << std::endl;
  return os;
}

bool operator==(const Pentagon& left,const Pentagon& right) {
	if(left.side_a == right.side_a)
		return true;
	else
		return false;
}

void Pentagon::Print() {
    std::cout << "Pentagon: a = " << side_a << std::endl;
}

Pentagon::~Pentagon() {
    std::cout << "Pentagon deleted" << std::endl;
}


