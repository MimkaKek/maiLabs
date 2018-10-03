#include <iostream>
#include <cstdio>
#include <cstdlib>

typedef struct Vector {
    int key;
    Vector* next;
} Vector;

int main() {
    int c;
    std::cin >> c;
    Vector* a = new Vector[c];
    for(int step = 0; step < c; step++) {
        a[step].key = step;
        a[step].next = NULL;
        printf("a[%d] = %d\n", step, a[step].key);
    }
    printf("a[%d] = %d\n", 2, a[2].key);
    printf("a[0] = %p\n", a[0]);
    printf("&a[0] = %p\n", &a[0]);
    printf("a[0].key = %d\n", a[0].key);
    printf("&a[0].key = %s\n", &a[0].key);
    printf("a = %p\n", a[0]);
    return 1;
}
