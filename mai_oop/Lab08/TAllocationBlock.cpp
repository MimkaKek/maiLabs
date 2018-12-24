#include "TAllocationBlock.h"
#include <memory>
#include <iostream>

TAllocationBlock::TAllocationBlock(size_t size,size_t count): _size(size),_count(count)  {
    _used_blocks = (char*)malloc(_size*_count);
    
    for(size_t i = 0; i < _count; ++i) {
        _queue_free_blocks.push(_used_blocks + (i * _size));
    }
    _free_count = _count;
    std::cout << "TAllocationBlock: Memory init" << std::endl;
}


void* TAllocationBlock::allocate() {
    char* result = nullptr;
    
    if(_free_count > 0) {
        result = _queue_free_blocks.pop();
        --_free_count;
        std::cout << "TAllocationBlock: Allocate " << (_count - _free_count) << " of " << _count << std::endl;
    } 
    else {
        std::cout << "TAllocationBlock: No memory exception :-)" << std::endl;
    }
    
    return result;
}

void TAllocationBlock::deallocate(void* pointer) {
    std::cout << "TAllocationBlock: Deallocate block "<< std::endl;
    _queue_free_blocks.push((char*)pointer);
    _free_count++;
}

bool TAllocationBlock::has_free_blocks() {
    return _free_count > 0;
}

TAllocationBlock::~TAllocationBlock() {
    if(_free_count < _count) 
        std::cout << "TAllocationBlock: Memory leak?" << std::endl;
    else  
        std::cout << "TAllocationBlock: Memory freed" << std::endl;
    free(_used_blocks);
}
