#include "Figure.h"

std::ostream& operator << (std::ostream &os, Figure& a) {
    a.Print();
    return os;
}
