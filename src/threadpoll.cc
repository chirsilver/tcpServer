#include "threadpoll.h"

Threadpoll::Threadpoll(int thread_cnt) {
    if(thread_cnt > MAX_THREADS) {
        thread_cnt = -1;
        fprintf(stderr, "ERROR: Too many threads to init.\n");
        assert(thread_cnt <= MAX_THREADS);
    }
    this->thread_cnt = thread_cnt;
    this->task_cnt = 0;
    this->task_que_head = nullptr;
    this->task_que_tail = nullptr;
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

    for(int i = 0; i < thread_cnt; i++) {
        void *res;
        pthread_join(threads[i], &res);
    }
}

void *start_routine(void *args) {
    Threadpoll *tp = (Threadpoll*)args;
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
    return 0;
}

void Threadpoll::threadpoll_exec() {
    Task *tk = nullptr;
    while(1) {
        mutex.lock();
        while(stat == START && task_que_head != nullptr) {
            cond.wait(&mutex);
        }
        if(stat == STOP) {
            mutex.unlock();
            pthread_exit(nullptr);
        }
        tk = task_que_head;
        task_que_head->set_next(task_que_head->next());
        mutex.unlock();
        tk->run();
        delete tk;
    }
}

void Threadpoll::add_task(Task *task) {
    mutex.lock();
    task_que_tail->set_next(task);
    task_que_tail = task;
    mutex.unlock();
    cond.signal();
}