#include <cmath>
#include <iostream>
#include <chrono>
#include <random>

class Timer {
private:
    // Псевдонимы типов используются для удобного доступа к вложенным типам
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1> >;
    
    std::chrono::time_point<clock_t> m_beg;
 
public:
    Timer() : m_beg(clock_t::now())
    {
    }
    
    void reset()
    {
        m_beg = clock_t::now();
    }
    
    double elapsed() const
    {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }
};

int main() {

    uint32_t n = 0, p = 0, sum = 0;
    
    std::cin >> n;
    std::cin >> p;
    std::cin >> sum;

    uint32_t* coin = new uint32_t[n];
    for(uint32_t i = 0; i < n; ++i) {
        coin[i] = 0;
    }
    Timer t;
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

    std::cout << "Time: " << t.elapsed() << " sec" << std::endl;
    return 0;
}
