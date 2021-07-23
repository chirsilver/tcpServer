#include "task.h"

Task::Task(void *(prograss)(void *args), void *args) {
    this->prograss = prograss;
    this->args = args;
}

Task::~Task() {}

void* Task::run() {
    return (this->prograss)(this->args);
}