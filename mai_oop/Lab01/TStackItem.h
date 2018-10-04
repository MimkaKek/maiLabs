#ifndef TSTACKITEM_H
#define	TSTACKITEM_H

#include "Rhombus.h"
class TStackItem {
public:
    TStackItem(const Rhombus& rhombus);
    TStackItem(const TStackItem& orig);
    friend std::ostream& operator<<(std::ostream& os, const TStackItem& obj);
    
    TStackItem* SetNext(TStackItem* next);
    TStackItem* GetNext();
    Rhombus GetRhombus() const;

    virtual ~TStackItem();
private:
    Rhombus rhombus;
    TStackItem *next;
};

#endif	/* TSTACKITEM_H */
