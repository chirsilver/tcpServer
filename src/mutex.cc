#include "mutex.h"

Mutex::Mutex() {
    pthread_mutex_init(&mutex, nullptr);
    is_locked = false;
}

Mutex::~Mutex() {
    if(is_locked) {
        unlock();
    }
    pthread_mutex_destroy(&mutex);
}

int Mutex::lock() {
    int ret = pthread_mutex_lock(&mutex);
    is_locked = true;
    return ret;
}

int Mutex::unlock() {
    int ret = pthread_mutex_unlock(&mutex);
    is_locked = false;
    return ret;
}

pthread_mutex_t* Mutex::get_mutex_pointer() {
    return &mutex;
}