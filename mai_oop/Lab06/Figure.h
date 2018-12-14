#ifndef FIGURE_H
#define	FIGURE_H

class Figure {
public:
    virtual double          Square() = 0;
    virtual void            Print() = 0;
    virtual std::ostream&   Print(std::ostream& os) = 0;
    
    friend std::ostream& operator<<(std::ostream &os, Figure& a){
        return a.Print(os);
    }
    virtual ~Figure() {};
};
#endif	/* FIGURE_H */

