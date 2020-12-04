#include <cmath>
#include <vector>
#include <stack>
#include <iostream>
#include <string>
#include <iomanip>

const unsigned int BASE = 100000;
const unsigned int BSIZE = 5;

class TBigInt {
    public:

        TBigInt(): base(BASE), bSize(BSIZE), fError(false) {};
        TBigInt(const TBigInt &toCopy) {
            base = toCopy.base;
            n = toCopy.n;
            bSize = toCopy.bSize;
            fError = toCopy.fError;
        };

        TBigInt(std::vector<long long> number) {

            base = BASE;
            n = number;
            bSize = 0;
            fError = false;
            
            size_t tmp = BASE - 1;
            while(tmp > 0) {
                tmp /= 10;
                ++bSize;
            }
        };

        TBigInt(size_t b) {

            base = BASE;
            bSize = 0;
            fError = false;
            
            size_t tmp = BASE - 1;
            while(tmp > 0) {
                tmp /= 10;
                ++bSize;
            }

            long long numb;
            do {
                numb = b % BASE;
                n.push_back(numb);
                b /= BASE;
            } while(b > 0);
        };

        TBigInt(size_t b, bool error) {

            base = BASE;
            bSize = 0;
            fError = error;
            
            size_t tmp = BASE - 1;
            while(tmp > 0) {
                tmp /= 10;
                ++bSize;
            }

            long long numb;
            do {
                numb = b % BASE;
                n.push_back(numb);
                b /= BASE;
            } while(b > 0);
        };
        
        ~TBigInt() {};

        //Бинарные операторы
        
        TBigInt          operator +  (const TBigInt &right) const;
        TBigInt          operator -  (const TBigInt &right) const;
        TBigInt          operator *  (const TBigInt &right) const;
        TBigInt          operator /  (const TBigInt &right) const;

        //Унарные операторы
        
        TBigInt&    operator += (const TBigInt &right); //TODO
        TBigInt&    operator -= (const TBigInt &right); //TODO
        TBigInt&    operator /= (const TBigInt &right); //TODO
        TBigInt&    operator *= (const TBigInt &right); //TODO
        TBigInt&    operator =  (const TBigInt &right);
        
        //Операции сравнения

        const    bool            operator >  (const TBigInt &right) const;
        const    bool            operator <  (const TBigInt &right) const;
        const    bool            operator >= (const TBigInt &right) const;
        const    bool            operator <= (const TBigInt &right) const;
        const    bool            operator == (const TBigInt &right) const;
        const    bool            operator != (const TBigInt &right) const;
        
        friend std::ostream& operator << (std::ostream &out, const TBigInt &numb);
        friend std::istream& operator >> (std::istream &in, TBigInt &numb);

        bool IsOdd() const;

        friend TBigInt pow(TBigInt a, TBigInt power);

    private:
        std::vector<long long>  n;
        size_t                  base;
        size_t                  bSize;
        bool                    fError;
};

bool TBigInt::IsOdd() const {
    return (n[0] % 2) == 1;
}

TBigInt pow(TBigInt a, TBigInt power) {

    TBigInt two = TBigInt(2);
    TBigInt zero = TBigInt(0);
    TBigInt res = TBigInt(1);

    if(a == zero) {
        if(power == zero) {
            return TBigInt(0, true);
        }
        else {
            return TBigInt(0);
        }
    }

    if(a == res) {
        return TBigInt(1);
    }
    
    while(power > zero) {
        if(power.IsOdd()) {
            res = res * a;
        }
        a = a * a;
        power = power / two;
    }
    
    return res;
}

TBigInt& TBigInt::operator = (const TBigInt &right) {
    base = right.base;
    n = right.n;
    return *this;
}

const bool TBigInt::operator == (const TBigInt &right) const {
    return n == right.n;
}

const bool TBigInt::operator != (const TBigInt &right) const {
    return n != right.n;
}

const bool TBigInt::operator > (const TBigInt &right) const {

    size_t ls = n.size(), rs = right.n.size();
    size_t step;
    
    if(ls != rs) {
        return ls > rs;
    }
    else {
        for(step = ls - 1; step > 0; --step) {
            if(n[step] != right.n[step]) {
                return n[step] > right.n[step];
            }
        }
        return n[0] > right.n[0];
    }
}

const bool TBigInt::operator >= (const TBigInt &right) const {

    size_t ls = n.size(), rs = right.n.size();
    size_t step;
    
    if(ls != rs) {
        return ls > rs;
    }
    else {
        for(step = ls - 1; step > 0; --step) {
            if(n[step] != right.n[step]) {
                return n[step] > right.n[step];
            }
        }
        return n[0] >= right.n[0];
    }
}

const bool TBigInt::operator < (const TBigInt &right) const {

    size_t ls = n.size(), rs = right.n.size();
    size_t step;
    
    if(ls != rs) {
        return ls < rs;
    }
    else {
        for(step = ls - 1; step > 0; --step) {
            if(n[step] != right.n[step]) {
                return n[step] < right.n[step];
            }
        }
        return n[0] < right.n[0];
    }
}

const bool TBigInt::operator <= (const TBigInt &right) const {

    size_t ls = n.size(), rs = right.n.size();
    size_t step;
    
    if(ls != rs) {
        return ls < rs;
    }
    else {
        for(step = ls - 1; step > 0; --step) {
            if(n[step] != right.n[step]) {
                return n[step] < right.n[step];
            }
        }
        return n[0] <= right.n[0];
    }
}

TBigInt TBigInt::operator + (const TBigInt &right) const {

    size_t i;
    size_t maxS = (this->n.size() > right.n.size()) ? this->n.size()  : right.n.size();
    size_t minS = (this->n.size() > right.n.size()) ? right.n.size() : this->n.size();

    std::vector<long long> answer;
    answer.resize(maxS + 1, 0);
    
    for(i = 0; i < minS; ++i) {
        answer[i] += this->n[i] + right.n[i];
        if(answer[i] >= base) {
            answer[i] -= base;
            answer[i + 1] += 1;
        }
    }

    for(i = minS; i < maxS; ++i) {
        answer[i] += (this->n.size() > right.n.size()) ? this->n[i] : right.n[i];
        if(answer[i] >= base) {
            answer[i] -= base;
            answer[i + 1] += 1;
        }
    }

    if(answer[answer.size() - 1] == 0 && answer.size() > 1) {
        answer.pop_back();
    }
    
    return TBigInt(answer);
}

TBigInt TBigInt::operator - (const TBigInt &right) const {

    size_t i, toErase = 0;
    size_t sA = n.size(), sB = right.n.size();

    std::vector<long long> answer;
    answer = n;
    if(*this >= right) {
        for(i = 0; i < sA; ++i) {
            answer[i] -= (i < sB ? right.n[i] : 0);
            if(answer[i] < 0) {
                answer[i] += base;
                answer[i + 1] -= 1;
            }
        }
    }
    else {
        return TBigInt(0, true);
    }
    
    while(answer.size() > 1 && answer.back() == 0) {
        answer.pop_back();
    }

    answer.erase(answer.end() - toErase, answer.end());
    return TBigInt(answer);
}

TBigInt TBigInt::operator / (const TBigInt &right) const {
    
    if(right.n.size() == 1) {
        if (right.n[0] == 1) {
            return TBigInt(*this);
        }
        else if (right.n[0] == 0) {
            return TBigInt(0, true);
        }
    }

    if(n.size() == 1) {
        if (n[0] == 0) {
            return TBigInt(0);
        }
    }

    if(*this < right) {
        return TBigInt(0);
    }
    
    std::vector<long long> res;
    res.resize(n.size());
    
    TBigInt cv = TBigInt(0);
    for (int i = (int) n.size() - 1; i >= 0; --i) {
        cv.n.insert(cv.n.begin(), n[i]);
        if (!cv.n.back())
            cv.n.pop_back();
        int x = 0, l = 0, r = BASE;
        while (l <= r) {
            int m = (l + r) / 2;
            TBigInt cur = right * TBigInt(m);
            if (cur <= cv) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        res[i] = x;
        cv = cv - right * TBigInt(x);
    }

    for(int i = res.size() - 1; res[i] == 0; --i) {
        res.pop_back();
    }
    
    return res;
}

TBigInt TBigInt::operator * (const TBigInt &right) const {

    size_t i, j, k;

    size_t sA = n.size(), sB = right.n.size();

    size_t sC = sA + sB;
    std::vector<long long> answer(sC, 0);

    for(i = 0; i < sB; ++i) {
        for(j = 0; j < sA; ++j) {
            answer[i + j] += n[j] * right.n[i];
            if(answer[i + j] >= base) {
                answer[i + j + 1] += (answer[i + j] / base);
                answer[i + j] %= base;
            }
        }
    }

    k = sC - 1;
    while(answer[k] == 0 && k > 0) {
        answer.pop_back();
        --k;
    }
    
    return TBigInt(answer);
}

std::ostream& operator << (std::ostream &out, const TBigInt &numb) {

    long long size = numb.n.size();
    if(size > 0 && !numb.fError) {
        out << (long long) numb.n[size - 1];
        for(long long p = size - 2; p >= 0; --p) {
            out << std::setfill('0') << std::setw(numb.bSize) << (long long) numb.n[p];
        }
    }
    else {
        out << "Error";
    }
    return out;
}

std::istream& operator >> (std::istream &in, TBigInt &numb) {

    numb.n.clear();
    size_t bSize = numb.bSize;
    std::string    str;
    long long  i, nSymb, check;
    
    in >> str;

    check = 0;
    while(str[check] == '0') {
        ++check;
    }

    if(check == str.size()) {
        numb.n.push_back(0);
    }
    else {
        for(i = str.size() - 1; i >= check; i -= bSize) {
            nSymb = i - check + 1;
            if(nSymb < bSize) {
                numb.n.push_back(atoi(str.substr(check, nSymb).c_str()));
            }
            else {
                numb.n.push_back(atoi(str.substr(i - bSize + 1, bSize).c_str()));
            }
            
        }
    }
    
    return in;
}

const unsigned char TESTS = 10;

int main() {
    
    TBigInt a, b;
    unsigned char comm;
    clock_t dt = 0, t0, t1;
    double avrTime;
    while(std::cin >> a >> b >> comm) {
        for(unsigned char iter = 0; iter < TESTS; ++iter) {
            t0 = clock();
            switch(comm) {
                case '+':
                {
                    std::cout << a + b << std::endl;
                    break;
                }
                case '-':
                {
                    std::cout << a - b << std::endl;
                    break;
                }
                case '*':
                {
                    std::cout << a * b << std::endl;
                    break;
                }
                case '/':
                {
                    std::cout << a / b << std::endl;
                    break;
                }
                case '^':
                {
                    std::cout << pow(a, b) << std::endl;
                    break;
                }
                case '>':
                {
                    std::cout << (a > b ? "true" : "false") << std::endl;
                    break;
                }
                case '<':
                {
                    std::cout << (a < b ? "true" : "false") << std::endl;
                    break;
                }
                case '=':
                {
                    std::cout << (a == b ? "true" : "false") << std::endl;
                    break;
                }
                default:
                {
                    std::cout << "ERROR: wrong command!" << std::endl;
                    return 1;
                }
            }
            t1 = clock();
            dt += t1 - t0;
        }
        avrTime = (double) dt / TESTS;
        avrTime /= CLOCKS_PER_SEC;
        std::cout << "Time: " << avrTime << std::endl;
    }
    return 0;
}
