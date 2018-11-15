#ifndef FIGURE_H
#define	FIGURE_H

class Figure {
public:
    virtual double Square() = 0;
    virtual void   Print() = 0;
    virtual ~Figure() {};
};
/*std::ostream& operator<<(std::ostream &os, Figure& a){
    dynamic_cast<Figure*>(&a)->Print();
    return os;
}*/
#endif	/* FIGURE_H */

