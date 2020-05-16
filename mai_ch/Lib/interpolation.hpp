#ifndef INTER_HPP
#define INTER_HPP

#include <iostream>
#include <vector>

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
};

//=========================================================================================================

#endif
