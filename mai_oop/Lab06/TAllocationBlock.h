#ifndef TALLOCATIONBLOCK_H
#define TALLOCATIONBLOCK_H

#include "TQueueSpec.h"
#include <cstdlib>

class TAllocationBlock {
public:
    TAllocationBlock(size_t size, size_t count);

    void*                      allocate();
    void                       deallocate(void *pointer);
    bool                       has_free_blocks();
    
    virtual ~TAllocationBlock();
private:
    size_t                 _size;
    size_t                 _count;
    
    char*                  _used_blocks;
    TQueueSpec             _queue_free_blocks;
    
    size_t                 _free_count;
};

#endif  /* TALLOCATIONBLOCK_H */
