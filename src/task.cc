#include "task.h"

Task::Task(void* (*foo)(void*), void* args) {
    this->prograss = foo;
    this->args = args;
    this->next = nullptr;
}

Task::~Task() {}

void* Task::run() {
    return prograss(args);
}