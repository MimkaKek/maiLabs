from random import randint
from math import gcd, pi
from functools import reduce
from datetime import datetime

MAGIC_NUMBER = 487382235506648564840107199192413681867587239828653
545406477588822435405764775982788867290488175802195211272137279254
075449003024560839618292390535501407609050238639766598383900922297
610271959554752478259182785991806370135230647201939344097195516435
438603272584360734684335554669921859642314670438294621187340324196
732562602497408907459047858027418101536161257362484646707984136223
986248632841895430142248772166147518572443486015665264246735010720
7026973065315203


def Func(x, a, b):
    return a * x ** 2 + b


def isPrime(N):
    if N in (0, 1):
        return False
    if N == 2:
        return True
    if N % 2 == 0:
        return False
    s = N - 1
    while s % 2 == 0:
        s //= 2
    for i in range(50):
        a = randint(1, N-1)
        exp = s
        mod = pow(a, exp, N)
        while exp != N - 1 and mod != 1 and mod != N - 1:
            mod = mod * mod % N
            exp *= 2
        if mod != N - 1 and exp % 2 == 0:
            return False
    return True


def findFactor(n):
    maxIter = pi / 4 * n
    a = randint(1, n - 1)
    b = randint(1, n - 1)
    i1 = randint(1, n - 1)
    i2 = i1
    divider = n
    iteration = 0
    while divider in (1, n):
        if iteration ** 2 > maxIter:
            a = randint(1, n - 1)
            b = randint(1, n - 1)
            i1 = randint(1, n - 1)
            i2 = i1
            iteration = 0
        i1 = Func(i1, a, b) % n
        i2 = Func(Func(i2, a, b), a, b) % n
        divider = gcd(abs(i1 - i2), n)
        iteration += 1
    return divider


def findPrimeFactor(n, factors):
    if isPrime(n):
        factors.append(n)
    else:
        tmp = n // findFactor(n)
        findPrimeFactor(tmp, factors)


def factor(n, factors):
    while n % 2 == 0:
        factors.append(2)
        n //= 2
    while n % 3 == 0:
        factors.append(3)
        n //= 3
    while n > 1:
        findPrimeFactor(n, factors)
        n //= factors[-1]


def find_all_factors(prime_factors, all_factors):
    if len(prime_factors) == 0:
        all_factors.append(1)
    elif len(all_factors) == 0:
        all_factors.append(1)
        all_factors.append(prime_factors[0])
    for i in range(1, len(prime_factors)):
        tmp = []
        for f in all_factors:
            if f * prime_factors[i] not in all_factors:
                tmp.append(f * prime_factors[i])
        all_factors += tmp
    all_factors.sort()


def AlgPollardRo(n):
    factors = []
    factor(n, factors)
    factors.sort()
    all_factors = []
    find_all_factors(factors, all_factors)
    return all_factors

def printFactors(num, factors):
    print("Original number: {0}".format(num))
    print("Factors:")
    print(*factors, sep='\n')

if __name__ == '__main__':
    n = 0
    with open('test1', 'r') as file:
        n = int(file.read())
    factors = AlgPollardRo(n)
    factors = factors[1:len(factors) - 2]
    printFactors(n, factors)

    factors.clear()
    with open('test2', 'r') as file:
        n = int(file.read())
    factors.append(gcd(n, MAGIC_NUMBER))
    factors.append(n / factors[0])
    printFactors(n, factors)
