#ifndef BIG_INT_H
#define BIG_INT_H

#include <iostream>
#include <string>

class BigInt {
	std::string value; // значение числа
	bool isNeg; // флаг отрицательности

	static BigInt karatsuba_mul(const BigInt &a, const BigInt &b); // умножение методом Карацубы

public:
	BigInt(); // конструктор умолчания (число равно нулю)
	BigInt(long x); // конструктор преобразования из обычного целого числа
	BigInt(const std::string &value); // конструктор преобразования из строки
	BigInt(const BigInt& bigInt); // конструктор копирования

	const std::string &getValue() const; // получение содержимого строки (строка модуля числа)

	const bool getIsNeg() const; // получение флага отрицательности числа
	void setIsNeg(bool isNeg); // установка флага отрицательности числа

	int sign() const; // получение знака числа
	const bool isEven() const; // проверка на чётность

	BigInt abs() const; // получение модуля числа
	BigInt pow(long n) const; // получение числа в степени n
	BigInt sqrt(long n = 2) const; // вычисление корня n-ой степени из числа

	// операции сравнения
	const bool operator==(const BigInt &bigInt) const;
	const bool operator!=(const BigInt &bigInt) const;

	const bool operator<(const BigInt &bigInt) const;
	const bool operator>(const BigInt &bigInt) const;
	const bool operator<=(const BigInt &bigInt) const;
	const bool operator>=(const BigInt &bigInt) const;

	// операция присваивания
	BigInt &operator=(const BigInt &bigInt);

	// унарные плюс и минус
	BigInt operator+() const &&;
	BigInt operator-() const &&;

	// арифметические операции
	BigInt operator+(const BigInt &bigInt) const;
	BigInt operator-(const BigInt &bigInt) const;
	BigInt operator*(const BigInt &bigInt) const;
	BigInt operator/(const BigInt &bigInt) const;
	BigInt operator%(const BigInt &bigInt) const;
	BigInt operator<<(size_t n) const;
	BigInt operator>>(size_t n) const;

	// краткая форма операций
	BigInt &operator+=(const BigInt &bigInt);
	BigInt &operator-=(const BigInt &bigInt);
	BigInt &operator*=(const BigInt &bigInt);
	BigInt &operator/=(const BigInt &bigInt);
	BigInt &operator%=(const BigInt &bigInt);
	BigInt &operator<<=(size_t n);
	BigInt &operator>>=(size_t n);

	// префиксная форма
	BigInt &operator++(); // ++v
	BigInt &operator--(); // --v

	// постфиксная форма
	BigInt operator++(int); // v++
	BigInt operator--(int); // v--


	friend std::ostream &operator<<(std::ostream &stream, const BigInt &bigInt); // вывод числа в выходной поток
	friend std::istream &operator>>(std::istream &stream, BigInt &bigInt); // ввод числа из входного потока
};

#endif