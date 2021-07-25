#include "cond.h"

Cond::Cond() {
    pthread_cond_init(&cond, nullptr);
}

Cond::~Cond() {
    pthread_cond_destroy(&cond);
}

int Cond::wait(Mutex *mutex) {
    return pthread_cond_wait(&cond, mutex->get_mutex_pointer());
}

int Cond::signal() {
    return pthread_cond_signal(&cond);
}

int Cond::bordcast() {
    return pthread_cond_broadcast(&cond);
}