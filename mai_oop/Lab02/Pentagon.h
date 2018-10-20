#ifndef PENTAGON_H
#define	PENTAGON_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Pentagon : public Figure{
public:
    Pentagon();
    Pentagon(std::istream &is);
    Pentagon(size_t a);
    Pentagon(const Pentagon& orig);

    friend bool operator==(const Pentagon& left,const Pentagon& right);
    double Square() override;
    void   Print() override;

    virtual ~Pentagon();
private:
    size_t side_a;
};

#endif	/* PENTAGON_H */

