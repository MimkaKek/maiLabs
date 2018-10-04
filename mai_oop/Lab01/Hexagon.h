#ifndef HEXAGON_H
#define	HEXAGON_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Hexagon : public Figure{
public:
    Hexagon();
    Hexagon(std::istream &is);
    Hexagon(size_t a);
    Hexagon(const Hexagon& orig);

    double Square() override;
    void   Print() override;

    virtual ~Hexagon();
private:
    size_t side_a;
};

#endif	/* HEXAGON_H */

