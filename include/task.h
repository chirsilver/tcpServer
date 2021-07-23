#ifndef _TASK_H_
#define _TASK_H_

class Task {
private:
    void *(*prograss)(void *args);
    Task* nxt;
    void *args;
public:
    Task(void *(prograss)(void *args), void *args);
    ~Task();
    void* run();
    Task* next();
    void set_next(Task *task);
};

#endif