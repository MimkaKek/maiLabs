#ifndef TARRAY_H
#define	TARRAY_H

#include "Rhombus.h"
#include "Pentagon.h"
#include "Hexagon.h"
#include "TIterator.h"
#include <memory>

template<class T> class TArray {
public:
	TArray();
  
	void Push(std::shared_ptr<T> figure);
  
	std::shared_ptr<T> Pop(int i);
  
  std::shared_ptr<T>* GetArray();
  
  void Print(int step) const;
  bool Empty();
  
  TIterator<TArray<T>,T> begin();
  TIterator<TArray<T>,T> end();
  
 	template <class A> friend std::ostream& operator<<(std::ostream& os, TArray<A>& array);
	virtual ~TArray();
private:
	unsigned int amount;
  std::shared_ptr<T>* array;
};

#endif	/* TARRAY_H */
