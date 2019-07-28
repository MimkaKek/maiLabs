#ifndef TARRAY_H
#define	TARRAY_H

#include "Rhombus.h"
#include "Pentagon.h"
#include "Hexagon.h"
#include <memory>

template<class T> class TArray {
public:
	
    TArray();
	
    void                    Add(std::shared_ptr<T>);
	std::shared_ptr<T>      Remove(int);
    void                    Print(int) const;
 	
    template <class A> friend std::ostream& operator << (std::ostream&, TArray<A>&);
	
    virtual ~TArray();
private:
	unsigned int            amount;
    std::shared_ptr<T>*     array;
};

#endif	/* TARRAY_H */
