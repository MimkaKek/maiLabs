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

    friend std::ostream& operator<<(std::ostream& os,const Hexagon& hexagon);
    friend bool operator==(const Hexagon& left,const Hexagon& right);
    
    double          Square() override;
    void            Print() override;
    std::ostream&   Print(std::ostream& os) override;

    virtual ~Hexagon();
private:
    int side_a;
};

#endif	/* HEXAGON_H */

