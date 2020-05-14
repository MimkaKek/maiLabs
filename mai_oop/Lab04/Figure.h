#ifndef FIGURE_H
#define FIGURE_H

#include <iostream>

class Figure {
public:
    virtual double Square() = 0;
    virtual void   Print() = 0;

    friend std::ostream& operator << (std::ostream &os, Figure& a);
    virtual ~Figure() {};
};

#endif  /* FIGURE_H */

