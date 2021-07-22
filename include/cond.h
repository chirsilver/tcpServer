#ifndef _COND_H_
#define _COND_H_

#include <pthread.h>

class Cond {
private:
    pthread_cond_t cond;
public:
    Cond();
    ~Cond();
    void wait(pthread_mutex_t *mutex);
    void signal();
    void bordcast();
};

#endif