#include "cond.h"

Cond::Cond(){
    pthread_cond_init(&this->cond, nullptr);
}

Cond::~Cond(){
    pthread_cond_destroy(&this->cond);
}

int Cond::wait(Mutex *mutex) {
    return pthread_cond_wait(&this->cond, mutex->get_mutex_pointer());
}

int Cond::signal() {
    return pthread_cond_signal(&this->cond);
}

int Cond::bordcast() {
    return pthread_cond_broadcast(&this->cond);
}