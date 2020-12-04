#include <iostream>
#include <stack>
#include <vector>
#include <ctime>

const unsigned char WEIGHT = 0;
const unsigned char COST = 1;
const unsigned char TESTS = 10;

int main() {

    
    clock_t dt = 0, t0, t1;
    double avrTime;
    
    int n = 0, m = 0;
    std::cin >> n;
    std::cin >> m;
    if(n == 0) {
        std::cout << 0 << std::endl;
        return 0;
    }
    std::vector<std::vector<long long>> item(n, std::vector<long long>(2));
    
    for(unsigned int i = 0; i < n; ++i) {
        std::cin >> item[i][WEIGHT];
        std::cin >> item[i][COST];
    }

    for(unsigned char iter = 0; iter < TESTS; ++iter) {
        t0 = clock();
        std::vector<std::vector<std::vector<long long>>> solve(n + 1, std::vector<std::vector<long long>>(m + 1));
        for(unsigned int i = 0; i <= n; ++i) {
            solve[i].assign(m + 1,std::vector<long long>(i + 1, -1));
            for(unsigned int j = 0; j <= m; ++j) {
                solve[i][j][0] = 0;
            }
        }

        for(unsigned int i = 0; i < n; ++i) {
            for(unsigned int w = 0; w <= m; ++w) {
                for(unsigned int p = 0; p <= i; ++p) {
                    int tmp = w + item[i][WEIGHT];
                    if(tmp <= m) {
                        if (solve[i][w][p] >= 0) {
                            solve[i + 1][tmp][p + 1] = solve[i][w][p] + item[i][COST];
                        }
                    }
                    solve[i + 1][w][p] = (solve[i + 1][w][p] > solve[i][w][p] ? solve[i + 1][w][p] : solve[i][w][p]);
                }
            }
        }

        long long check = -1, answer = 0, sum;
        for(unsigned int p = 0; p <= n; ++p) {
            sum = p * solve[n][m][p];
            if(answer < sum) {
                check = p;
                answer = sum;
            }
        }
        std::cout << answer << std::endl;
        if(check < 0) {
            return 0;
        }

        int number = n, weight = m;
        std::stack<int> stack;

        while(number != 0) {
            if(solve[number][weight][check] == 0) {
                break;
            }
            if(solve[number - 1][weight][check] != solve[number][weight][check] || check == number) {
                --check;
                weight -= item[number - 1][WEIGHT];
                stack.push(number);
            }
            --number;
        }

        if(!stack.empty()) {
            std::cout << stack.top();
            stack.pop();
            while(!stack.empty()) {
                std::cout << ' ' << stack.top();
                stack.pop();
            }
            std::cout << std::endl;
        }
        t1 = clock();
        dt += t1 - t0;
    }

    
    avrTime = (double) dt / TESTS;
    avrTime /= CLOCKS_PER_SEC;
    std::cout << "Time: " << avrTime << std::endl;
    return 0;
}
