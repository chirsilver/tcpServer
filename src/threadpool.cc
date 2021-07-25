#include "threadpool.h"

Threadpool::Threadpool() {
    status = THREADPOOLSTOP;
    mutex = nullptr;
    cond = nullptr;
    tasks = nullptr;
    threads = nullptr;
    thread_num = 0;
}

Threadpool::~Threadpool() {
    if(status == THREADPOOLSTART) {
        destory();
    }
}

static void* routine(void *args) {
    Threadpool *tp = (Threadpool*)args;
    tp->exec();
    return nullptr;
}

int Threadpool::create(int thread_num) {
    if(thread_num > MAX_THREADS) {
        fprintf(stderr, "Error: Can't create so many threads.\n");
        return -1;
    }
    mutex = new Mutex();
    cond = new Cond();
    this->thread_num = thread_num;
    threads = new pthread_t[thread_num];
    if(!threads) {
        delete mutex;
        delete cond;
        delete[] threads;
        fprintf(stderr, "Error: Create threads faild.\n");
        return 0;
    }
    for(int i = 0; i < thread_num; i++) {
        threads[i] = pthread_create(&threads[i], nullptr, routine, this);
    }
    status = THREADPOOLSTART;
    return 0;
}

void Threadpool::destory() {
    if(status == THREADPOOLSTOP) return;
    status = THREADPOOLSTOP;
    mutex->lock();
    cond->bordcast();
    mutex->unlock();

    void *ret;
    for(int i = 0; i < thread_num; i++) {
        pthread_join(threads[i], &ret);
    }
    delete mutex;
    delete cond;
    while(tasks) {
        Task *tk = tasks;
        tasks = tasks->next;
        delete tk;
    }
    delete[] threads;
}

void Threadpool::add_task(void* (*foo)(void*), void *args) {
    mutex->lock();
    Task *tk = tasks;
    if(tk == nullptr) {
        tk = new Task(foo, args);
        tasks = tk;
    } else {
        while(tk->next != nullptr) {
            tk = tk->next;
        }
        tk->next = new Task(foo, args);
    }
    mutex->unlock();
    cond->signal();
}

void Threadpool::exec() {
    while(1) {
        mutex->lock();
        while(nullptr == tasks || status == THREADPOOLSTOP) {
            cond->wait(mutex);
        }
        if(nullptr == tasks) {
            mutex->unlock();
            continue;
        }
        if(status == THREADPOOLSTOP) {
            mutex->unlock();
            pthread_exit(nullptr);
            return;
        }
        Task *tk = tasks;
        tasks = tasks->next;
        mutex->unlock();
        tk->run();
        delete tk;
    }
}