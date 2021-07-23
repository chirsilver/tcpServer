#include "mutex.h"

Mutex::Mutex() {
    pthread_mutex_init(&this->mutex, nullptr);
    is_locked = false;
}

Mutex::~Mutex() {
    while(is_locked) unlock();
    pthread_mutex_destroy(&this->mutex);
}

int Mutex::lock() {
    is_locked = true;
    return pthread_mutex_lock(&this->mutex);
}

int Mutex::unlock() {
    is_locked = false;
    return pthread_mutex_unlock(&this->mutex);
}

int Mutex::trylock() {
    is_locked = true;
    return pthread_mutex_trylock(&this->mutex);
}

bool Mutex::get_stat() {
    return is_locked;
}

pthread_mutex_t* Mutex::get_mutex_pointer() {
    return &this->mutex;
}