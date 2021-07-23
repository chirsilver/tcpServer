#include "threadpoll.h"

Threadpoll::Threadpoll() {
    thread_cnt = 1;
    this->task_cnt = 0;
    this->tasks = nullptr;
    this->threads = nullptr;
}

Threadpoll::~Threadpoll() {
    if(stat == START) destory();
}

void Threadpoll::destory() {
    mutex.lock();
    stat = STOP;
    mutex.unlock();
    cond.bordcast();
    while(tasks != nullptr) {
        Task *task = tasks;
        tasks = tasks->nxt;
        delete task;
    }
    for(int i = 0; i < thread_cnt; i++) {
        void *res;
        pthread_join(threads[i], &res);
    }
}

void *start_routine(void *args) {
    Threadpoll *tp = (Threadpoll*)args;
    printf("thread is run\n");
    tp->threadpoll_exec();
    return nullptr;
}

int Threadpoll::threadpoll_init() {
    threads = new pthread_t[thread_cnt];

    for(int i = 0; i < thread_cnt; i++) {
        int res = pthread_create(&threads[i], nullptr, start_routine, this);
        if(res != 0) {
            fprintf(stderr, "ERROR: threadpoll_init() error.\n");
            return -1;
        }
    }
    stat = START;

    fprintf(stdout, "threadpoll create %d thread success...\n", thread_cnt);
    return 0;
}

void Threadpoll::threadpoll_exec() {
    Task *tk = nullptr;
    while(1) {
        mutex.lock();
        while(stat == STOP || tasks == nullptr) {
            cond.wait(&mutex);
        }
        if(stat == STOP) {
            mutex.unlock();
            pthread_exit(nullptr);
        }
        else if(tasks == nullptr) {
            mutex.unlock();
        }
        else {
            tk = tasks;
            tasks = tasks->nxt;
            mutex.unlock();
            tk->run();
            delete tk;
        }
    }
}

void Threadpoll::add_task(Task *task) {
    mutex.lock();
    Task *tk = tasks;
    while(tk != nullptr) tk = tk->nxt;
    tk = task;
    mutex.unlock();
    cond.signal();
}