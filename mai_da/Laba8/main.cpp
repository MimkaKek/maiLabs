#include <cmath>
#include <iostream>

int main() {

    uint32_t n = 0, p = 0, sum = 0;
    
    std::cin >> n;
    std::cin >> p;
    std::cin >> sum;

    uint32_t* coin = new uint32_t[n];
    for(uint32_t i = 0; i < n; ++i) {
        coin[i] = 0;
    }

    uint32_t step = n - 1;
    while(sum != 0) {

        uint32_t current = pow(p, step);
        
        while(sum >= current) {
            sum -= current;
            coin[step] += 1;
        }

        --step;
    }

    for(uint32_t i = 0; i < n; ++i) {
        std::cout << coin[i] << std::endl;
    }
    return 0;
}
