//               testadec.cpp
#include <iostream>
#include <iomanip>
#include "fsm.h"
using namespace std;

int main() {
    tFSM Adec;
    addrange(Adec, 0, '1', '9', 2);
    addstr  (Adec, 0, "-+", 1);
    addstr  (Adec, 0, "0", 3);
    addstr  (Adec, 0, ".", 4);
    addrange(Adec, 1, '1', '9', 2);
    addstr  (Adec, 1, "0", 3);
    addstr  (Adec, 1, ".", 4);
    addrange(Adec, 2, '0', '9', 2); 
    addstr  (Adec, 2, ".", 4);
    addstr  (Adec, 2, "Ee", 6);
    addstr  (Adec, 3, ".", 4);
    addstr  (Adec, 3, "Ee", 6);
    addrange(Adec, 4, '0', '9', 5);
    addrange(Adec, 5, '0', '9', 5);
    addstr  (Adec, 5, "eE", 6);
    addstr  (Adec, 6, "+-", 7);
    addrange(Adec, 7, '0', '9', 8);
    addrange(Adec, 8, '0', '9', 8);
    
    Adec.final(2);
    Adec.final(5);
    Adec.final(8);
    
    cout << "*** xxx Adec " << "size=" << Adec.size() << " ***\n" << endl;
    
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

