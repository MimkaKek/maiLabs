#include <iostream>
#include <stack>
#include <utility>

bool PRINT = false;

int main() {

    size_t n = 0, m = 0;
    
    std::cin >> n;
    if(n < 1 || n > 100) {
        std::cout << "ERROR: bad input!" << std::endl;
        return 1;
    }
    std::cin >> m;
    if(m < 1 || m > 5000) {
        std::cout << "ERROR: bad input!" << std::endl;
        return 1;
    }

    size_t* weight = new size_t[n];
    size_t* cost   = new size_t[n];
    

    for(size_t i = 0; i < n; ++i) {
        std::cin >> weight[i];
        std::cin >> cost[i];
    }

    size_t* answ  = new size_t[(n + 1) * (m + 1)]; // [(m + 1)*i + j]
    size_t* check = new size_t[(n + 1) * (m + 1)];
    size_t* mCost = new size_t[(n + 1) * (m + 1)];
    size_t* count = new size_t[(n + 1) * (m + 1)];
    

    if(PRINT) {
        for(size_t i = 0; i < n; ++i) {
            std::cout << "| " << weight[i] << " | " << cost[i] << " |" << std::endl;
        }   
    }

    size_t pos = 0;
    
    for(size_t i = 0; i <= n; ++i) {
        for(size_t w = 0; w <= m; ++w) {

            size_t cPos = (m + 1)*i + w;

            if(i == 0 || w == 0) {
                answ [cPos] = 0;
                count[cPos] = 0;
                check[cPos] = 0;
                mCost[cPos] = 0;
            }
            else {

                size_t yPos = (m + 1)*(i - 1) + (w - weight[i - 1]);
                size_t nPos = (m + 1)*(i - 1) + w;
                
                if(weight[i - 1] > w) {
                    answ [cPos] = answ [nPos];
                    mCost[cPos] = mCost[nPos];
                    count[cPos] = count[nPos];
                    check[cPos] = 0;
                }
                else {
                    
                    size_t yTake = (count[yPos] + 1) * (mCost[yPos] + cost[i - 1]);
                    size_t nTake = answ[nPos];

                    if(yTake > nTake) {
                        answ [cPos] = yTake;
                        mCost[cPos] = mCost[yPos] + cost[i - 1];
                        count[cPos] = count[yPos] + 1;
                        check[cPos] = 1;
                    }
                    else {
                        answ [cPos] = nTake;
                        mCost[cPos] = mCost[nPos];
                        count[cPos] = count[nPos];
                        check[cPos] = 0;
                    }
                }
            }
            
        }
    }
    
    
    if(PRINT) {
        std::cout << "Cost matrix" << std::endl;
        for(size_t i = 0; i <= n; ++i) {
            for(size_t j = 0; j <= m; ++j) {
                std::cout << answ[(m + 1) * i + j] << ' ';
            }
            std::cout << std::endl;
        }
    }

    if(PRINT) {
        std::cout << "Check matrix:" << std::endl;
        for(size_t i = 0; i <= n; ++i) {
            for(size_t j = 0; j <= m; ++j) {
                std::cout << check[(m + 1) * i + j] << ' ';
            }
            std::cout << std::endl;
        }
    }

    if(PRINT) {
        std::cout << "Count matrix:" << std::endl;
        for(size_t i = 0; i <= n; ++i) {
            for(size_t j = 0; j <= m; ++j) {
                std::cout << count[(m + 1) * i + j] << ' ';
            }
            std::cout << std::endl;
        }
    }

    std::stack<size_t> stack;
        
    std::cout << answ[(m + 1)*n + m] << std::endl;
    
    pos = m;
    for(size_t i = n; i > 0; --i) {
        if(check[(m + 1)*i + pos]) {
            pos -= weight[i - 1];
            stack.push(i);
        }
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
    
    delete[] mCost;
    delete[] check;
    delete[] answ;
    delete[] count;
    delete[] weight;
    delete[] cost;
    return 0;
}
