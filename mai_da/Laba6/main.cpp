#include <cmath>
#include <vector>
#include <stack>
#include <iostream>
#include <string>

class BigInt {
    public:

        BigInt(): limit(10) {};
        BigInt(const BigInt &toCopy) {
            limit = toCopy.limit;
            n = toCopy.n;
        };

        BigInt(std::vector<unsigned char> nn, size_t l) {
            limit = l;
            n = nn;
        };

        BigInt(size_t l) {
            limit = l;
            n = {0};
        };
        
        ~BigInt() {};
        
        friend  const    BigInt          operator +  (const BigInt &left, const BigInt &right);
        friend  const    BigInt          operator -  (const BigInt &left, const BigInt &right);
        friend  const    BigInt          operator *  (const BigInt &left, const BigInt &right); //TODO
        friend  const    BigInt          operator *  (const BigInt &left, const size_t &right); //TODO
        friend  const    BigInt          operator /  (const BigInt &left, const BigInt &right); //TODO
        friend  const    BigInt          operator /  (const BigInt &left, const size_t &right); //TODO
        friend  const    bool            operator >  (const BigInt &left, const BigInt &right);
        friend  const    bool            operator <  (const BigInt &left, const BigInt &right);
        friend  const    bool            operator >= (const BigInt &left, const BigInt &right);
        friend  const    bool            operator <= (const BigInt &left, const BigInt &right);
        friend  const    bool            operator == (const BigInt &left, const BigInt &right);
        friend  const    bool            operator != (const BigInt &left, const BigInt &right);
        friend std::ostream& operator << (std::ostream &out, const BigInt &numb);
        friend std::istream& operator >> (std::istream &in, BigInt &numb);

        
        BigInt&    operator += (const BigInt &right); //TODO
        BigInt&    operator -= (const BigInt &right); //TODO
        BigInt&    operator /= (const BigInt &right); //TODO
        BigInt&    operator *= (const BigInt &right); //TODO
        BigInt&    operator =  (const BigInt &right) {
            limit = right.limit;
            n = right.n;
            return *this;
        }

    private:
        std::vector<unsigned char>  n;
        size_t                      limit;
};

const bool   operator == (const BigInt &left, const BigInt &right) {

    size_t ls = left.n.size(), rs = right.n.size();
    size_t step;
    
    if(ls != rs) {
        return false;
    }
    else {
        for(step = ls - 1; step > 0; --step) {
            if(left.n[step] != right.n[step]) {
                return false;
            }
        }
        return left.n[0] == right.n[0];
    }
}

const bool   operator != (const BigInt &left, const BigInt &right) {

    size_t ls = left.n.size(), rs = right.n.size();
    size_t step;
    
    if(ls == rs) {
        return false;
    }
    else {
        for(step = ls - 1; step > 0; --step) {
            if(left.n[step] == right.n[step]) {
                return false;
            }
        }
        return left.n[0] != right.n[0];
    }
}

const bool   operator >  (const BigInt &left, const BigInt &right) {

    size_t ls = left.n.size(), rs = right.n.size();
    size_t step;
    
    if(ls < rs) {
        return false;
    }
    else if(ls > rs) {
        return true;
    }
    else {
        for(step = ls - 1; step > 0; --step) {
            if(left.n[step] > right.n[step]) {
                return true;
            }
            else if(left.n[step] < right.n[step]) {
                return false;
            }
        }
        return left.n[0] > right.n[0];
    }
}

const bool   operator >= (const BigInt &left, const BigInt &right) {

    size_t ls = left.n.size(), rs = right.n.size();
    size_t step;
    
    if(ls < rs) {
        return false;
    }
    else if(ls > rs) {
        return true;
    }
    else {
        for(step = ls - 1; step > 0; --step) {
            if(left.n[step] > right.n[step]) {
                return true;
            }
            else if(left.n[step] < right.n[step]) {
                return false;
            }
        }
        return left.n[0] >= right.n[0];
    }
}

const bool   operator <  (const BigInt &left, const BigInt &right) {

    size_t ls = left.n.size(), rs = right.n.size();
    size_t step;
    
    if(ls < rs) {
        return true;
    }
    else if(ls > rs) {
        return false;
    }
    else {
        for(step = ls - 1; step > 0; --step) {
            if(left.n[step] > right.n[step]) {
                return false;
            }
            else if(left.n[step] < right.n[step]) {
                return true;
            }
        }
        return left.n[0] < right.n[0];
    }
}

const bool   operator <= (const BigInt &left, const BigInt &right) {

    size_t ls = left.n.size(), rs = right.n.size();
    size_t step;
    
    if(ls < rs) {
        return true;
    }
    else if(ls > rs) {
        return false;
    }
    else {
        for(step = ls - 1; step > 0; --step) {
            if(left.n[step] > right.n[step]) {
                return false;
            }
            else if(left.n[step] < right.n[step]) {
                return true;
            }
        }
        return left.n[0] <= right.n[0];
    }
}

const BigInt operator +  (const BigInt &left, const BigInt &right) {

    size_t i;
    size_t limit = left.limit;
    size_t maxS = (left.n.size() > right.n.size()) ? left.n.size()  : right.n.size();
    size_t minS = (left.n.size() > right.n.size()) ? right.n.size() : left.n.size();

    std::vector<unsigned char> answer;
    answer.resize(maxS + 1, 0);
    
    for(i = 0; i < minS; ++i) {
        answer[i] = left.n[i] + right.n[i];
        if(answer[i] >= limit) {
            answer[i] -= limit;
            answer[i + 1] += 1;
        }
    }

    for(i = minS; i < maxS; ++i) {
        answer[i] = (left.n.size() > right.n.size()) ? left.n[i] : right.n[i];
        if(answer[i] >= limit) {
            answer[i] -= limit;
            answer[i + 1] += 1;
        }
    }

    if(answer[answer.size() - 1] == 0 && answer.size() > 1) {
        answer.pop_back();
    }
    
    return BigInt(answer, limit);
}

const BigInt operator - (const BigInt &left, const BigInt &right) {
    
    size_t i, toErase = 0;
    size_t limit = left.limit;
    size_t size  = (left.n.size() > right.n.size()) ? right.n.size() : left.n.size();

    std::vector<unsigned char> answer;
    answer = left.n;
    if(left >= right) {
        for(i = 0; i < size; ++i) {
            if(answer[i] < right.n[i]) {
                answer[i + 1] -= 1;
                answer[i] += limit;
            }
            answer[i] -= right.n[i];
        }
    }
    else {
        std::cout << "Error";
        return BigInt();
    }
    
    i = answer.size() - 1;
    while(i > 0) {
        if(answer[i] == 0) {
            ++toErase;
        }
        --i;
    }

    answer.erase(answer.end() - toErase, answer.end());
    return BigInt(answer, limit);
}

const BigInt operator *  (const BigInt &left, const size_t &right) {
    
    size_t limit = left.limit;
    size_t r     = right;
    size_t i, j, k;
    std::vector<unsigned char> answer;
    std::vector<unsigned char> shift;
    std::vector<unsigned char> numb;

    if(r == 0) {
        answer.push_back(0);
        return BigInt(answer, limit);
    }
    
    while(r > 0) {
        numb.push_back(r % 10);
        r /= 10;
    }

    for(i = 0; i < numb.size(); ++i) {
        shift.clear();
        for(j = 0; j < left.n.size(); ++j) {
            shift.push_back(left.n[j] * numb[i]);
        }

        size_t mp = shift.size() + i;
        
        for(j = i; j < mp; ++j) {
            if(j == answer.size()) {
                answer.push_back(shift[j - i]);
            }
            else {
                answer[j] += shift[j - i];
            }
        }
        k = i;
        while(answer[k] >= limit) {

            if((k + 1) == answer.size()) {
                answer.push_back(0);
            }
            
            while(answer[k] >= limit) {
                answer[k] -= limit;
                answer[k + 1] += 1;
            }
            
            ++k;
        }
    }
    
    return BigInt(answer, limit);
}

const BigInt operator *  (const BigInt &left, const BigInt &right) {

    size_t limit = left.limit;
    size_t i, j, k;
    std::vector<unsigned char> answer;
    std::vector<unsigned char> shift;

    shift.resize(left.n.size(), 0);
    answer.resize(left.n.size() + right.n.size(), 0);

    for(i = 0; i < right.n.size(); ++i) {
        
        for(j = 0; j < left.n.size(); ++j) {
            shift[j] = left.n[j] * right.n[i];
        }

        size_t mp = shift.size() + i;
        
        for(j = i; j < mp; ++j) {
            answer[j] += shift[j - i];
        }
        
        k = i;
        while(answer[k] >= limit) {

            while(answer[k] >= limit) {
                answer[k] -= limit;
                answer[k + 1] += 1;
            }
            
            ++k;
        }
    }

    for(i = answer.size() - 1; answer[i] == 0 && i > 0; --i) {
        answer.pop_back();
    }
    
    return BigInt(answer, limit);
}

std::ostream& operator << (std::ostream &out, const BigInt &numb) {

    size_t size = numb.n.size();
    if(size > 0) {
         for(size_t p = size - 1; p > 0; --p) {
            out << (unsigned int) numb.n[p];
        }
        out << (unsigned int) numb.n[0];
    }
    return out;
}

std::istream& operator >> (std::istream &in, BigInt &right) {
    std::string                 str;
    unsigned long               i, check = 0;
    std::stack<unsigned char>   stack;
    right.n.clear();

    in >> str;
    
    while(str[check] == '0') {
        ++check;
    }

    if(check == str.size()) {
        right.n.push_back(0);
    }
    else {
        for(i = str.size() - 1; i > check; --i) {
            right.n.push_back(str[i] - '0');
        }
        right.n.push_back(str[check] - '0');
    }
    

    return in;
}

int main() {

    BigInt a, b;
    unsigned char comm;

    while(true) {
        std::cin >> a;
        std::cin >> b;
        std::cin >> comm;
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
                //std::cout << a / b << std::endl;
                break;
            }
            case '^':
            {
                //std::cout << a / b << std::endl;
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
