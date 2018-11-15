#ifndef PENTAGON_H
#define	PENTAGON_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Pentagon : public Figure{
public:
    Pentagon();
    Pentagon(std::istream &is);
    Pentagon(int a);
    Pentagon(const Pentagon& orig);

    friend std::ostream& operator<<(std::ostream& os,const Pentagon& pentagon);
    friend bool operator==(const Pentagon& left,const Pentagon& right);
    double Square() override;
    void   Print() override;

    virtual ~Pentagon();
private:
    int side_a;
};

#endif	/* PENTAGON_H */

