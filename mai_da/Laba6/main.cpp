#include <cmath>
#include <vector>
#include <stack>
#include <iostream>
#include <string>
#include <iomanip>

const unsigned int BASE = 100000;
const unsigned int BSIZE = 5;

class BigInt {
    public:

        BigInt(): base(BASE), bSize(BSIZE), fError(false) {};
        BigInt(const BigInt &toCopy) {
            base = toCopy.base;
            n = toCopy.n;
            bSize = toCopy.bSize;
            fError = toCopy.fError;
        };

        BigInt(std::vector<long long> number) {

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

        BigInt(size_t b) {

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

        BigInt(size_t b, bool error) {

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
        
        ~BigInt() {};

        //Бинарные операторы
        
        BigInt          operator +  (const BigInt &right) const;
        BigInt          operator -  (const BigInt &right) const;
        BigInt          operator *  (const BigInt &right) const;
        BigInt          operator /  (const BigInt &right) const;

        //Унарные операторы
        
        BigInt&    operator += (const BigInt &right); //TODO
        BigInt&    operator -= (const BigInt &right); //TODO
        BigInt&    operator /= (const BigInt &right); //TODO
        BigInt&    operator *= (const BigInt &right); //TODO
        BigInt&    operator =  (const BigInt &right);
        
        //Операции сравнения

        const    bool            operator >  (const BigInt &right) const;
        const    bool            operator <  (const BigInt &right) const;
        const    bool            operator >= (const BigInt &right) const;
        const    bool            operator <= (const BigInt &right) const;
        const    bool            operator == (const BigInt &right) const;
        const    bool            operator != (const BigInt &right) const;
        
        friend std::ostream& operator << (std::ostream &out, const BigInt &numb);
        friend std::istream& operator >> (std::istream &in, BigInt &numb);

        bool IsOdd() const;

        friend BigInt pow(BigInt a, BigInt power);

    private:
        std::vector<long long>  n;
        size_t                  base;
        size_t                  bSize;
        bool                    fError;
};

bool BigInt::IsOdd() const {
    return (n[0] % 2) == 1;
}

BigInt pow(BigInt a, BigInt power) {

    BigInt two = BigInt(2);
    BigInt zero = BigInt(0);
    BigInt res = BigInt(1);

    if(power == zero) {
        return BigInt(0, true);
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

BigInt& BigInt::operator = (const BigInt &right) {
    base = right.base;
    n = right.n;
    return *this;
}

const bool BigInt::operator == (const BigInt &right) const {

    size_t ls = this->n.size(), rs = right.n.size();
    size_t step;
    
    if(ls != rs) {
        return false;
    }
    else {
        for(step = ls - 1; step > 0; --step) {
            if(this->n[step] != right.n[step]) {
                return false;
            }
        }
        return this->n[0] == right.n[0];
    }
}

const bool BigInt::operator != (const BigInt &right) const {
    return !(*this == right);
}

const bool BigInt::operator > (const BigInt &right) const {

    size_t ls = this->n.size(), rs = right.n.size();
    size_t step;
    
    if(ls < rs) {
        return false;
    }
    else if(ls > rs) {
        return true;
    }
    else {
        for(step = ls - 1; step > 0; --step) {
            if(this->n[step] > right.n[step]) {
                return true;
            }
            else if(this->n[step] < right.n[step]) {
                return false;
            }
        }
        return this->n[0] > right.n[0];
    }
}

const bool BigInt::operator >= (const BigInt &right) const {

    size_t ls = this->n.size(), rs = right.n.size();
    size_t step;
    
    if(ls < rs) {
        return false;
    }
    else if(ls > rs) {
        return true;
    }
    else {
        for(step = ls - 1; step > 0; --step) {
            if(this->n[step] > right.n[step]) {
                return true;
            }
            else if(this->n[step] < right.n[step]) {
                return false;
            }
        }
        return this->n[0] >= right.n[0];
    }
}

const bool BigInt::operator < (const BigInt &right) const {

    size_t ls = this->n.size(), rs = right.n.size();
    size_t step;
    
    if(ls < rs) {
        return true;
    }
    else if(ls > rs) {
        return false;
    }
    else {
        for(step = ls - 1; step > 0; --step) {
            if(this->n[step] > right.n[step]) {
                return false;
            }
            else if(this->n[step] < right.n[step]) {
                return true;
            }
        }
        return this->n[0] < right.n[0];
    }
}

const bool BigInt::operator <= (const BigInt &right) const {

    size_t ls = this->n.size(), rs = right.n.size();
    size_t step;
    
    if(ls < rs) {
        return true;
    }
    else if(ls > rs) {
        return false;
    }
    else {
        for(step = ls - 1; step > 0; --step) {
            if(this->n[step] > right.n[step]) {
                return false;
            }
            else if(this->n[step] < right.n[step]) {
                return true;
            }
        }
        return this->n[0] <= right.n[0];
    }
}

BigInt BigInt::operator + (const BigInt &right) const {

    size_t i;
    size_t base = this->base;
    size_t maxS = (this->n.size() > right.n.size()) ? this->n.size()  : right.n.size();
    size_t minS = (this->n.size() > right.n.size()) ? right.n.size() : this->n.size();

    std::vector<long long> answer;
    answer.resize(maxS + 1, 0);
    
    for(i = 0; i < minS; ++i) {
        answer[i] = this->n[i] + right.n[i];
        if(answer[i] >= base) {
            answer[i] -= base;
            answer[i + 1] += 1;
        }
    }

    for(i = minS; i < maxS; ++i) {
        answer[i] = (this->n.size() > right.n.size()) ? this->n[i] : right.n[i];
        if(answer[i] >= base) {
            answer[i] -= base;
            answer[i + 1] += 1;
        }
    }

    if(answer[answer.size() - 1] == 0 && answer.size() > 1) {
        answer.pop_back();
    }
    
    return BigInt(answer);
}

BigInt BigInt::operator - (const BigInt &right) const {

    size_t i, toErase = 0;
    size_t base = this->base;
    size_t size  = (this->n.size() > right.n.size()) ? right.n.size() : this->n.size();

    std::vector<long long> answer;
    answer = this->n;
    if(*this >= right) {
        for(i = 0; i < size; ++i) {
            answer[i] -= right.n[i];
            if(answer[i] < 0) {
                answer[i] += base;
                answer[i + 1] -= 1;
            }
        }
    }
    else {
        return BigInt(0, true);
    }
    
    i = answer.size() - 1;
    while(i > 0 && answer[i] == 0) {
        ++toErase;
        --i;
    }

    answer.erase(answer.end() - toErase, answer.end());
    return BigInt(answer);
}

BigInt BigInt::operator / (const BigInt &right) const {
    
    if(right.n.size() == 1) {
        if (right.n[0] == 1) {
            return BigInt(*this);
        }
        else if (right.n[0] == 0) {
            return BigInt(0, true);
        }
    }

    if(n.size() == 1) {
        if (n[0] == 0) {
            return BigInt(0);
        }
    }

    std::vector<long long> res;
    res.resize(n.size());
    
    BigInt cv = BigInt(0);
    for (int i = (int) n.size() - 1; i >= 0; --i) {
        cv.n.insert(cv.n.begin(), n[i]);
        if (!cv.n.back())
            cv.n.pop_back();
        int x = 0, l = 0, r = BASE;
        while (l <= r) {
            int m = (l + r) / 2;
            BigInt cur = right * BigInt(m);
            if (cur <= cv) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        res[i] = x;
        cv = cv - right * BigInt(x);
    }

    for(int i = res.size() - 1; res[i] == 0; --i) {
        res.pop_back();
    }
    
    return res;
}

BigInt BigInt::operator * (const BigInt &right) const {

    size_t i, j, k;
    std::vector<long long> answer;
    std::vector<long long> shift;

    shift.resize(n.size(), 0);
    answer.resize(n.size() + right.n.size(), 0);

    for(i = 0; i < right.n.size(); ++i) {
        
        for(j = 0; j < n.size(); ++j) {
            shift[j] = n[j] * right.n[i];
        }

        size_t mp = shift.size() + i;
        
        for(j = i; j < mp; ++j) {
            answer[j] += shift[j - i];
        }
        
        k = i;
        while(answer[k] >= base) {
            answer[k + 1] += (answer[k] / base);
            answer[k] %= base;
            ++k;
        }
    }

    for(i = answer.size() - 1; answer[i] == 0 && i > 0; --i) {
        answer.pop_back();
    }
    
    return BigInt(answer);
}

std::ostream& operator << (std::ostream &out, const BigInt &numb) {

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

std::istream& operator >> (std::istream &in, BigInt &numb) {

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

int main() {

    BigInt a, b;
    unsigned char comm;

    while(std::cin >> a >> b >> comm) {
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
                std::cerr << "ERROR: wrong command!" << std::endl;
                exit(1);
            }
        }
    }
    return 0;
}
