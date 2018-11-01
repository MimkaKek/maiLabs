#include <string>

class TTreeItem {
    public:
        TTreeItem();
        void Add(std::string str, int key);
        void Remove(std::string str, int key);
        void Print();
        ~TTreeItem();
    private:
        str::string word;
        unsigned long long int key;
        TTreeItem* sibling;
        TTreeItem* child;
};
