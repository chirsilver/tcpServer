#ifndef _THREADPOOL_H
#define _THREADPOOL_H_

#include "mutex.h"
#include "cond.h"
#include "task.h"
#include <pthread.h>
#include <stdio.h>

#define THREADPOOLSTOP 0
#define THREADPOOLSTART 1
#define MAX_THREADS 20

class Threadpool {
private:
    Mutex *mutex;
    Cond *cond;
    bool status;
    Task *tasks;
    int thread_num;
    pthread_t *threads;
public:
    Threadpool();
    ~Threadpool();
    int create(int thread_num);
    void destory();
    void add_task(void* (*foo)(void*), void *args);
    void exec();
};

#endif