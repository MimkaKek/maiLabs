#include "TQueueSpec.h"

TQueueSpec::TQueueSpec() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

void TQueueSpec::push(char* item) {
    std::shared_ptr<TQueueItemSpec> other(new TQueueItemSpec(item));
    if(tail == nullptr) {
        head = other;
        tail = other;
        size = 1;
    }
    else {
        tail->SetNext(other);
        tail = tail->GetNext();
        ++size;
    }
}

bool TQueueSpec::empty() {
    return head == nullptr;
}

char* TQueueSpec::pop() {
    char* result = nullptr;
    if (head != nullptr) {
        result = head->GetValue();
        head = head->GetNext();
    }
    --size;
    return result;
}
 
TQueueSpec::~TQueueSpec() {
    head = nullptr;
}
