#include <iostream>
#include "TAhiKorasik.h"

int main()
{
    TAhoCorasick ak;

    ak.AddString("testtttovaya_stroka!");
    ak.AddString("testtttovaya_stroka");
    ak.AddString("testtttovaya_strok");
    ak.AddString("testtttovaya_stro");
    ak.AddString("testtttovaya_str");
    ak.AddString("testtttovaya_st");
    ak.AddString("testtttovaya_s");
    ak.AddString("testtttovaya_");
    ak.AddString("testtttovaya");
    ak.AddString("testtttovay");
    ak.AddString("testtttova");
    ak.AddString("testtttov");
    ak.AddString("testttto");
    ak.AddString("testttt");
    ak.AddString("testtt");
    ak.AddString("testt");
    ak.AddString("test");
    ak.AddString("tes");
    ak.AddString("te");
    ak.AddString("t");

    ak.Initialize();

    ak.Search("testtttovaya_stroka!");

    std::cin.get();

    return 0;
}
