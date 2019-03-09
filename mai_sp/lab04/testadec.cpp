//               testadec.cpp
#include <iostream>
#include <iomanip>
#include "fsm.h"
using namespace std;

enum {
    BAD_END = 10,
    GOOD_END
};

int main() {
    tFSM Adec;
    addrange(Adec, 0, '1', '9', 5);
    addstr(Adec, 0, "-+", 1);
    addstr(Adec, 0, "0", 2);
    addrange(Adec, 1, '1', '9', 5);
    addstr(Adec, 1, "0", 2);
    addrange(Adec, 2, '0', '9', BAD_END);
    addstr(Adec, 2, ".", 3);
    addstr(Adec, 2, "Ee", 4);
    addrange(Adec, 3, '0', '9', 3);
    addstr(Adec, 3, "Ee", 4);
    addstr(Adec, 4, "+-", 6);
    addrange(Adec, 5, '0', '9', 5);
    addstr(Adec, 5, "eE", 4);
    addstr(Adec, 5, ".", 3);
    addrange(Adec, 6, '0', '9', 6);
    Adec.final(0);
    Adec.final(1);
    Adec.final(2);
    Adec.final(3);
    Adec.final(4);
    Adec.final(5);
    Adec.final(6);
    Adec.final(BAD_END);
    cout << "*** xxx Adec "
    << "size=" << Adec.size()
    << " ***\n";
    cout << endl;
    
    while(true) {
        char input[81];
        cout << ">";
        cin.getline(input,81);
        if(!*input) {
            break; 
        }
        int res = Adec.apply(input);
        cout << setw(res ? res + 1 : 0) << "^"
        << endl;
    }   
    return 0;
}

