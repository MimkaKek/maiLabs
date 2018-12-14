#include "TQueueItemSpec.h"
#include <iostream>

TQueueItemSpec::TQueueItemSpec(char* item) {
    this->item = item;
    this->next = nullptr;
    std::cout << "Stack item: created" << std::endl;
}

std::shared_ptr<TQueueItemSpec> TQueueItemSpec::SetNext(std::shared_ptr<TQueueItemSpec> next) {
    std::shared_ptr<TQueueItemSpec> old = this->next;
    this->next = next;
    return old;
}

char* TQueueItemSpec::GetValue() const {
    return this->item;
}

std::shared_ptr<TQueueItemSpec> TQueueItemSpec::GetNext() {
    return this->next;
}

TQueueItemSpec::~TQueueItemSpec() {
    next = nullptr;
    std::cout << "Memory get" << std::endl;
}
