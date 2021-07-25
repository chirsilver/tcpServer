#ifndef _COND_H_
#define _COND_H_

#include "mutex.h"
#include <pthread.h>

class Cond {
private:
    pthread_cond_t cond;
public:
    Cond();
    ~Cond();
    int wait(Mutex *mutex);
    int signal();
    int bordcast();
};

#endif