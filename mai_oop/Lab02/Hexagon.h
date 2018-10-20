#ifndef HEXAGON_H
#define	HEXAGON_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Hexagon : public Figure{
public:
    Hexagon();
    Hexagon(std::istream &is);
    Hexagon(int a);
    Hexagon(const Hexagon& orig);

    friend bool operator==(const Hexagon& left,const Hexagon& right);
    double Square() override;
    void   Print() override;

    virtual ~Hexagon();
private:
    int side_a;
};

#endif	/* HEXAGON_H */

