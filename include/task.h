#ifndef _TASK_H_
#define _TASK_H_

class Task {
private:
    void* (*prograss)(void*);
    void *args;
public:
    Task(void* (*foo)(void*), void* args);
    ~Task();
    void* run();
    Task *next;
};

#endif