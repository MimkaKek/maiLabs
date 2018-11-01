#ifndef RHOMBUS_H
#define	RHOMBUS_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Rhombus : public Figure{
public:
    Rhombus();
    Rhombus(std::istream &is);
    Rhombus(int d1, int d2);
    Rhombus(const Rhombus& orig);
    friend std::ostream& operator<<(std::ostream& os,const Rhombus& rhombus);
    friend bool operator==(const Rhombus& left,const Rhombus& right);
    double Square() override;
    void   Print() override;

    virtual ~Rhombus();
private:
    int side_d1;
    int side_d2;
};

#endif	/* RHOMBUS_H */

