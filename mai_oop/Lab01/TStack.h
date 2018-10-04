#ifndef TSTACK_H
#define	TSTACK_H

#include "Rhombus.h"
#include "TStackItem.h"

class TStack {
public:
    TStack();
    TStack(const TStack& orig);
    
    void push(Rhombus &&rhombus);
    bool empty();
    Rhombus pop();
    friend std::ostream& operator<<(std::ostream& os,const TStack& stack);
    virtual ~TStack();
private:
    
    TStackItem *head;
};

#endif	/* TSTACK_H */
