#ifndef TARRAY_H
#define	TARRAY_H

#include "Rhombus.h"
#include <memory>

class TArray {
public:
	TArray();
	void Add(std::shared_ptr<Figure> figure);
	void Remove(std::shared_ptr<Figure> figure);
  void Print(int step);
	unsigned int GetAmount() const;
	friend std::ostream& operator<<(std::ostream& os, TArray& array);
	virtual ~TArray();
private:
	unsigned int amount;
  std::shared_ptr<Figure>* array;
};

#endif	/* TARRAY_H */
