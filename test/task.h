#ifndef _TASK_H_
#define _TASK_H_

class Task {
private:
    void *(*prograss)(void *args);
    void *args;
public:
    Task(void *(prograss)(void *args), void *args);
    ~Task();
    void* run();
    Task* nxt;
};

#endif