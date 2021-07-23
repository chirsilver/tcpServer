#ifndef _THREADPOLL_H_
#define _THREADPOLL_H_

#include "task.h"
#include "mutex.h"
#include "cond.h"
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#define THREADS 100
#define DEFAULT_THREADS 10
#define START 1
#define STOP 0

class Threadpoll {
private:
    int task_cnt;
    int thread_cnt;
    Mutex mutex;
    Cond cond;
    Task *tasks;
    pthread_t *threads;

    bool stat;
    void destory();

public:
    Threadpoll();
    ~Threadpoll();
    void add_task(Task *task);
    int threadpoll_init();
    void threadpoll_exec();
};

#endif