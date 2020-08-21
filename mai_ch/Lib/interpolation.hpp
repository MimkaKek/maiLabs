#ifndef INTER_HPP
#define INTER_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include "matrix.hpp"

//=========================================================================================================

class TTableOfPoints;

class TTableItem {
    public:
        TTableItem();
        TTableItem(double nx, double ny, TTableItem* nPrev, TTableItem* nNext);
        ~TTableItem();

        double GetY();
        double GetX();
        
        friend bool                         operator == (const TTableItem& a, const TTableItem& b);
        friend bool                         operator != (const TTableItem& a, const TTableItem& b);
        friend std::ostream&                operator << (std::ostream &out, const TTableOfPoints &table);
        
    private:
    
        double                              x;
        double                              y;
        TTableItem*                         prev;
        TTableItem*                         next;

        friend TTableOfPoints;
};

bool operator == (const TTableItem& a, const TTableItem& b);
bool operator != (const TTableItem& a, const TTableItem& b);

bool CX1 (TTableItem& a, TTableItem& b);
bool CX2 (TTableItem& a, TTableItem& b);
bool CY1 (TTableItem& a, TTableItem& b);
bool CY2 (TTableItem& a, TTableItem& b);
bool CC  (TTableItem& a, TTableItem& b);

class TTableOfPoints {
    public:
        TTableOfPoints();
        ~TTableOfPoints();
        
        void SetCompare(bool (*n)(TTableItem&, TTableItem&));
        void SetHead(size_t n);
        void AddPoint(double nx, double ny);
        void RemovePoint(size_t n);
        void PrintTable();
        bool IsEmpty();
        size_t getSize();
        double getP(size_t n, char t = 'x');
        
        friend  std::ostream&               operator << (std::ostream &out, const TTableOfPoints &table);
                std::pair<double, double>   operator [] (const size_t n);

    private:
        size_t                              size;
        size_t                              pos;
        TTableItem*                         head;
        TTableItem*                         end;
        TTableItem                          begin;

        bool (*Compare)(TTableItem&, TTableItem&);
};

std::ostream& operator << (std::ostream &out, const TTableOfPoints &table);

//=========================================================================================================

class TLagrangeInter {
    public:
        TLagrangeInter();
        TLagrangeInter(TTableOfPoints* nTable);
        ~TLagrangeInter();

        void Update();

        double  operator () (double& x);
    private:
        TTableOfPoints*                     table;
        std::vector<double>                 coef;
};

class TNewtonInter {
    public:
        TNewtonInter();
        TNewtonInter(TTableOfPoints* nTable);
        ~TNewtonInter();

        void Update();
        
        double  operator () (double& x);
    private:
        TTableOfPoints*                     table;
        std::vector<std::vector<double>>    coef;
};

class TSplines {
    public:
        TSplines();
        TSplines(TTableOfPoints* nTable);
        ~TSplines();
        
        void Update();
        
        double  operator () (double& x);
    private:
        TTableOfPoints*                     table;
        std::vector<std::vector<double>>    coef;

        double H(size_t i);
};

//=========================================================================================================

void Derivative(TTableOfPoints* table, double x);
void Integral(double (*func)(double), double x1, double x2, double h1, double h2);

//=========================================================================================================

#endif
