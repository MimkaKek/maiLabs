#include <iostream>
#include "TAhiKorasik.h"

int main()
{
    TAhoCorasick ak;

    ak.AddString("abb");

    ak.Initialize();

    ak.Search("acbcbacacacbcbacccbaacaaaaaa");

    std::cin.get();

    return 0;
}
