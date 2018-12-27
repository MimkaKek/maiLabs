#ifndef TQUEUESPEC_H
#define TQUEUESPEC_H

#include "TQueueItemSpec.h"
#include <memory>

class TQueueSpec {
public:
    TQueueSpec();
    
    void push(char* item);
    bool empty();
    char* pop();
    
    virtual ~TQueueSpec();
private:
    size_t size;
    std::shared_ptr<TQueueItemSpec> tail;
    std::shared_ptr<TQueueItemSpec> head;
};

#endif
