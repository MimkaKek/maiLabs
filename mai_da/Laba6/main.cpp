#include <cmath>
#include <vector>
#include <stack>
#include <iostream>


class BigInt {
    public:

        BigInt(): m(1), limit(10) {};
        BigInt(BigInt &toCopy) {
            limit = toCopy.limit;
            n = toCopy.n;
            m = toCopy.m;
        };

        BigInt(std::vector<unsigned long> &nn, int &mm, size_t l) {
            limit = l;
            n = nn;
            m = mm;
        };

        BigInt(int mm, size_t l) {
            limit = l;
            m = mm;
        };
        
        ~BigInt();
        
        friend  const    BigInt          operator +  (const BigInt &left, const BigInt &right);
        friend  const    BigInt          operator -  (const BigInt &left, const BigInt &right);
        friend  const    BigInt          operator *  (const BigInt &left, const BigInt &right);
        friend  const    BigInt          operator /  (const BigInt &left, const BigInt &right);
        friend  const    bool            operator == (const BigInt &left, const BigInt &right);
        friend  const    bool            operator != (const BigInt &left, const BigInt &right);
        friend std::ostream& operator << (std::ostream &out, const BigInt &numb);
        friend std::istream& operator >> (std::istream &in, BigInt &numb);

        
        BigInt&    operator += (const BigInt &right);
        BigInt&    operator -= (const BigInt &right);
        BigInt&    operator /= (const BigInt &right);
        BigInt&    operator *= (const BigInt &right);
        BigInt&    operator =  (const BigInt &right) {
            limit = right.limit;
            n = right.n;
            m = right.m;
            return *this;
        }

    private:
        std::vector<unsigned long>  n;
        int                         m;
        size_t                      limit;
};

const BigInt operator + (const BigInt &left, const BigInt &right) {

    BigInt answer;
    size_t step = 0;
    size_t size = (left.n.size() > right.n.size()) ? right.n.size() : left.n.size();

    answer.n.push_back(0);
    while(step < size) {
        answer.n[step] += left.n[step] + right.n[step];
        if(answer.n[step] >= answer.limit) {
            answer.n[step] -= answer.limit;
            answer.n.push_back(1);
        }
        ++step;
    }

    if(answer.n[step - 1] > answer.limit) {
        answer.n[step - 1] -= answer.limit;
        answer.n.push_back(1);
    }

    return BigInt(answer);
}

std::ostream& operator << (std::ostream &out, const BigInt &numb) {

    size_t size = numb.n.size();
    for(size_t p = size - 1; p > 0; --p) {
        out << numb.n[p];
    }
    out << numb.n[0];
    
    return out;
}
std::istream& operator >> (std::istream &in, BigInt &numb) {
    unsigned char ch = '0';
    unsigned long number = 0;
    std::stack<unsigned char> stack;
    while(ch >= '0' && ch <= '9') {
        in >> ch;
        stack.push(ch);
    }
    
    while(!stack.empty()) {
        number = (stack.top() - '0');
        numb.n.push_back(number);
    }

    return in;
}

int main() {

    BigInt a, b;
    std::cin >> a;
    std::cin >> b;

    std::cout << a + b;
    return 0;
}
