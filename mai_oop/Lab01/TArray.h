#ifndef TARRAY_H
#define	TARRAY_H

#include "Rhombus.h"

class TArray {
public:
	TArray();
	void Add(Rhombus rhombus);
	void Remove(Rhombus rhombus);
	Rhombus* GetRhombus(int pos) const;
	unsigned int GetAmount() const;
	friend std::ostream& operator<<(std::ostream& os, TArray& array);
	virtual ~TArray();
private:
	unsigned int amount;
	Rhombus** array;
};

#endif	/* TARRAY_H */
