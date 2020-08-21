#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <cstddef>

const size_t POINTS  = 20;
const double EPSILON = 0.001;

enum MENU {
    BEGIN_MENU,
    TASK_MENU,
    PART1_MENU,
    PART2_MENU
};

enum MTypes {
    ZERO_MATRIX,
    UNIT_MATRIX
};

enum ConvTypes {
    STRING,
    COLUMN
};

enum Reverse {
    VIA_LU,
    VIA_M
};

enum NormTypes {
    TYPE_1,
    TYPE_2,
    TYPE_C
};

enum Swap {
    BACK,
    FORWARD
};

#endif
