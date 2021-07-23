#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <pthread.h>

class Mutex {
private:
    pthread_mutex_t mutex;
    bool is_locked;
public:
    Mutex();
    ~Mutex();
    int lock();
    int unlock();
    int trylock();
    bool get_stat();
    pthread_mutex_t *get_mutex_pointer();
};

#endif