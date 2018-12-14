#ifndef TQUEUEITEMSPEC_H
#define TQUEUEITEMSPEC_H

#include <memory>

class TQueueItemSpec {
public:
    TQueueItemSpec(char* figure);
    
    std::shared_ptr<TQueueItemSpec>  SetNext(std::shared_ptr<TQueueItemSpec> next);
    std::shared_ptr<TQueueItemSpec>  GetNext();
    char*                            GetValue() const;
    
    virtual ~TQueueItemSpec();
private:
    char*                            item;
    std::shared_ptr<TQueueItemSpec>  next;
};

#endif
