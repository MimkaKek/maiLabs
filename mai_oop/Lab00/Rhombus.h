#ifndef RHOMBUS_H
#define	RHOMBUS_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Rhombus : public Figure{
public:
    Rhombus();
    Rhombus(std::istream &is);
    Rhombus(size_t d1, size_t d2);
    Rhombus(const Rhombus& orig);

    double Square() override;
    void   Print() override;

    virtual ~Rhombus();
private:
    size_t side_d1;
    size_t side_d2;
};

#endif	/* RHOMBUS_H */

