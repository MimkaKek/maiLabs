#ifndef TARRAY_H
#define	TARRAY_H

#include "Rhombus.h"
#include "Pentagon.h"
#include "Hexagon.h"
#include <memory>

template<class T> class TArray {
public:
	TArray();
	void Add(std::shared_ptr<T> figure);
	std::shared_ptr<T> Remove(int i);
  void Print(const int step);
	unsigned int GetAmount() const;
 	template <class A> friend std::ostream& operator<<(std::ostream& os, TArray<A>& array);
	virtual ~TArray();
private:
	unsigned int amount;
  std::shared_ptr<T>* array;
};

#endif	/* TARRAY_H */
