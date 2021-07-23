#include "task.h"

Task::Task() {
    nxt = nullptr;
}

Task::~Task() {}

void* Task::run() {
    return (this->prograss)(this->args);
}

Task* Task::next() {
    return this->nxt;
}

void Task::set_next(Task  *task) {
    this->nxt = task;
}