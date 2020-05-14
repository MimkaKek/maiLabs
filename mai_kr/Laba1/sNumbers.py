from random import randint
from math import gcd, pi
from functools import reduce
from datetime import datetime

# Функция для Ро-алгоритма Полларда
def func(x, a, b):
    return a * x ** 2 + b


# Проверка числа на простоту
def is_prime(N):
    # Тривиальные случаи
    if N in (0, 1):
        return False
    if N == 2:
        return True
    if N % 2 == 0:
        return False

    # Тест Миллера - Рабина
    # Сводится к такому определению:
    # Пусть N - простое число и N - 1 = 2^s * d, где d - нечётно.
    # Тогда для любого целого положительного a < N выполняется хотя бы одно из условий
    # 1. a^d = 1 (mod n)
    # 2. Существует целое число r < s такое что a^(2^r * d) = -1 (mod n)

    # Ищем такую нечётную d, что N - 1 = 2^s * d
    d = N - 1

    # По сути тут можно вычислить s, но в данной реализации он не нужен
    while d % 2 == 0:
        d //= 2

    # Проведём 50 таких тестов
    for i in range(50):
        
        # Задаём начальные данные для теста
        a = randint(1, N - 1)
        exp = d
        mod = pow(a, exp, N)

        # В этом моменте мы косвенно используем s
        # Проявляется в условии exp != N - 1
        while exp != N - 1 and mod != 1 and mod != N - 1:
            mod = mod * mod % N
            exp *= 2

        if mod != N - 1 and exp % 2 == 0:
            return False

    return True


def find_divider(n):
    # Подстраховка на случай, если поиск затянулся
    # для текущих значений a, b, x
    maxiterssq = n / 2;
    iters = 0
    
    # Ро-алгоритм Полларда
    x = randint(1, n - 1)
    y = x
    
    d = 1
    a = randint(1, n - 1)
    b = randint(1, n - 1)

    # Выйдем из этого цикла только тогда, когда найдём делитель d
    # Гарантируется с некоторой вероятностью, что число n - составное
    while d in (1, n):
        
        x = func(x, a, b) % n
        y = func(func(y, a, b), a, b) % n
        d = gcd(abs(x - y), n)

        iters += 1

        if iters ** 2 > maxiterssq:

            # Заново генерируем числа для функции func(x, a, b)
            a = randint(1, n - 1)
            b = randint(1, n - 1)
            x = randint(1, n - 1)
            y = x
            iters = 0
    return d


def find_prime_divider(n, dividers):
    if is_prime(n):
        dividers.append(n)
    else:
        tmp = n // find_divider(n)
        find_prime_divider(tmp, dividers)


def divider(n, dividers):
    while n % 2 == 0:
        dividers.append(2)
        n //= 2
    while n % 3 == 0:
        dividers.append(3)
        n //= 3
    while n > 1:
        find_prime_divider(n, dividers)
        n //= dividers[-1]


def find_all_dividers(prime_dividers, all_dividers):
    all_dividers.append(1)
    all_dividers.append(prime_dividers[0])
    for i in range(1, len(prime_dividers)):
        tmp = []
        for f in all_dividers:
            if f * prime_dividers[i] not in all_dividers:
                tmp.append(f * prime_dividers[i])
        all_dividers += tmp
    all_dividers.sort()

def ro_pollard(n):
    dividers = []
    divider(n, dividers)
    dividers.sort()
    all_dividers = []
    
    all_dividers.append(1)
    all_dividers.append(dividers[0])
    for i in range(1, len(dividers)):
        tmp = []
        for f in all_dividers:
            if f * dividers[i] not in all_dividers:
                tmp.append(f * dividers[i])
        all_dividers += tmp
    all_dividers.sort()
    
    return all_dividers


def get_info(num, dividers, start_time):
    print("Time: {0}".format(datetime.now() - start_time))
    print("Original number: {0}".format(num))
    print("Dividers:")
    print(*dividers, sep='\n')

# Начало

n = 0

n = eval(input())
start_time = datetime.now()
dividers = ro_pollard(n)
dividers = dividers[1:len(dividers) - 2]
get_info(n, dividers, start_time)

dividers.clear()

n = eval(input())
start_time = datetime.now()
dividers = ro_pollard(n)
dividers = dividers[1:len(dividers) - 2]
get_info(n, dividers, start_time)
