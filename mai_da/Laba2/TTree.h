#include "TTreeItem.h"
#include <string>

class TTree {
    public:
        TTree();
        void Add(std::string str, unsigned long long int key);
        void Remove(std::string str, unsigned long long int key);
        void Show();
        ~TTree();
    private:
        TTreeItem* node;
};
