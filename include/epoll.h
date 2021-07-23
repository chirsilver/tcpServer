#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <sys/epoll.h>
#include <unistd.h>

#define MAX_EPOLL_EVENTS 1000
#define MAX_EPOLL_FDSIZE 2000

class Epoll {
private:
    int epfd;
    int listenfd;
    epoll_event events[MAX_EPOLL_EVENTS];
public:
    Epoll();
    ~Epoll();
    int epoll_init(int servfd, epoll_event *event);
    int epoll_run();
    int handle_events(int cnt);
    int add_event(int fd, epoll_event *event);
    int mod_event(int fd, epoll_event *event);
    int del_event(int fd, epoll_event *event);
};

#endif