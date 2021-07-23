#include "epoll.h"

int Epoll::epoll_init(int servfd, epoll_event *event) {
    listenfd = servfd;
    epfd = epoll_create(MAX_EPOLL_FDSIZE);
    if(epfd < 0) return -1;
    return add_event(listenfd, event);
}

int Epoll::epoll_run() {
    return epoll_wait(epfd, events, MAX_EPOLL_EVENTS, -1);
}

int Epoll::add_event(int fd, epoll_event *event) {
    return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, event);
}

int Epoll::mod_event(int fd, epoll_event *event) {
    return epoll_ctl(epfd, EPOLL_CTL_MOD, fd, event);
}

int Epoll::del_event(int fd, epoll_event *event) {
    return epoll_ctl(epfd, EPOLL_CTL_DEL, fd, event);
}

Epoll::~Epoll() {
    close(epfd);
}