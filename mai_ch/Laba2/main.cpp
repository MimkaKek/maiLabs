#include <iostream>
#include <cmath>

const size_t POINTS = 20;

double Func(double x) {
    return pow(2, x) - pow(x, 2) - 0.5;
}

double dFunc(double x) {
    return log(2) * pow(2, x) - 2*x;
}

double ddFunc(double x) {
    return log(2) * log(2) * pow(2, x) - 2;
}

double Fi1(double x) {
    return sqrt(pow(2, x) - 0.5);
}

double dFi1(double x) {
    return (log(2)*pow(2, x)) / (sqrt(pow(2, x) - 0.5));
}

double Fi2(double x) {
    return log10(x * x + 0.5)/log10(2);
}

double dFi2(double x) {
    return 2*x/(log10(2)*log(10)*(x * x + 0.5));
}

double SF1(double x1, double x2) {
    return (x1*x1 + 4)*x2 - 8;
}

double SF2(double x1, double x2) {
    return (x1 - 1)*(x1 - 1) + (x2 - 1)*(x2 - 1) - 4;
}

double NewtonMethod(double a, double b, double begin, double eps = 0.001) {

    if(Func(a)*Func(b) > 0) {
        std::cout << "Error: F(a)*F(b) > 0" << std::endl;
        return 0;
    }

    if(begin < a || begin > b) {
        std::cout << "Error: x0 are out of borders!" << std::endl;
        std::cout << "Take x0 = " << a << std::endl;
        begin = a;
        if(dFunc(begin)*ddFunc(begin) <= 0) {
            std::cout << "Error: dF(a)*d(dF(a)) <= 0!" << std::endl;
            std::cout << "Take x0 = " << b << std::endl;
            begin = b;
            if(dFunc(begin)*ddFunc(begin) <= 0) {
                std::cout << "Error: dF(x0)*d(dF(x0)) <= 0" << std::endl;
                return 0;
            }
        }
    }
    else if(dFunc(begin)*ddFunc(begin) <= 0) {
        std::cout << "Error: dF(x0)*d(dF(x0)) <= 0" << std::endl;
        return 0;
    }

    double x = begin;
    double nx = x - Func(x) / dFunc(x);
    double delta = nx - x;
    
    delta = (delta >= 0) ? delta : -delta;

    for(size_t iter = 0; delta > eps; ++iter) {

        x = nx;
        nx = x - Func(x) / dFunc(x);

        delta = nx - x;
        delta = (delta >= 0) ? delta : -delta;
    }

    return nx;
}

double SimpleIterations(double a, double b, double begin, double eps = 0.001) {

    if(begin < a || begin > b) {
        std::cout << "Error: x0 are out of borders!" << std::endl;
        std::cout << "Take x0 = " << a << std::endl;
        begin = a;
    }
    
    double x = a;
    double dx = (b - a) / (POINTS - 1);    
    double nx;
    double delta;
    double point[4];
    double dMax[2]  = {0, 0};
    bool   check[2] = {true, true};
    
    for(size_t p = 1; p < POINTS; ++p) {

        point[0] = Fi1(x);
        point[1] = Fi2(x);
        point[2] = dFi1(x);
        point[3] = dFi2(x);
        point[2] = (point[2] >= 0) ? point[2] : -point[2];
        point[3] = (point[3] >= 0) ? point[3] : -point[3];
        dMax[0] = (dMax[0] < point[2]) ? point[2] : dMax[0];
        dMax[1] = (dMax[1] < point[3]) ? point[3] : dMax[1];
        
        if(point[0] < a || point[0] > b) {
            check[0] = false;
        }

        if(point[1] < a || point[1] > b) {
            check[1] = false;
        }

        x += dx;
    }

    if(dMax[0] > 1) {
        check[0] = false;
    }

    if(dMax[1] > 1) {
        check[1] = false;
    }

    if(check[0] || check[1]) {
        std::cout << "All conditions are completed!" << std::endl;

        x = begin;
        
        if(check[0]) {
            
            
            nx = Fi1(x);
            delta = nx - x;
            delta = (delta >= 0) ? delta : -delta;
        
            for(size_t iter = 0; delta > eps; ++iter) {
        
                x = nx;
                nx = Fi1(x);
        
                delta = nx - x;
                delta = (delta >= 0) ? delta : -delta;
                
            }
        }
        else {

            x = Fi2(x);
            delta = nx - x;
            delta = (delta >= 0) ? delta : -delta;
        
            for(size_t iter = 0; delta > eps; ++iter) {
        
                x = nx;
                nx = Fi2(x);
        
                delta = nx - x;
                delta = (delta >= 0) ? delta : -delta;
                
            }
        }
    }
    else {
        std::cout << "Error: bad borders!" << std::endl;
    }

    return nx;
}

int main() {
    std::cout << NewtonMethod(2, 6, 7, 0.001) << std::endl;
    std::cout << SimpleIterations(3, 5, 4) << std::endl;
    return 0;
}
