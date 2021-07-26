#include "epoll.h"
#include "threadpool.h"

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <map>

#define ASSERT(exep) ((exep)?({void(0);}):({return -1;}))
#define ASSERT_DO(exep, do) ((exep)?({void(0);}):({(do); return -1;}))

#define START 1
#define STOP 0
#define THREAD_NUM 5
#define EPOLL_QUE 5
#define MAX_LISTENQUE 5
#define BUFFER_SIZE 1024
#define IPADDR "127.0.0.1"
#define PORT 8787

// static function
int Socket(const char *ip, const int port);
int Bind();
int Listen();
int Run();
void Over();
void Accept();
void Handle_stdin();
void* Read(void *args);
void* Write(void *args);

// static data
int servfd = -1, status = STOP;
sockaddr_in servaddr;
Epoll *ep = nullptr;
Threadpool *tp = nullptr;
std::map<int, sockaddr_in> mmap;
Mutex mmap_mutex;

typedef struct Node {
    int fd;
    sockaddr_in addr;
} client;

int main(int argc, char **argv) {
    int ret = Socket(IPADDR, PORT);
    ASSERT_DO(ret == 0, Over());
    puts("socket create success...");

    ret = Bind();
    ASSERT_DO(ret == 0, Over());
    printf("socket bind on ip:%s, port:%d success...\n", IPADDR, PORT);

    ret = Listen();
    ASSERT_DO(ret == 0, Over());

    puts("socket start listen...");

    Run();

    return 0;
}

int Run() {
    printf("Run() thread is %d\n", gettid());
    tp = new Threadpool();
    int ret = tp->create(THREAD_NUM);
    ASSERT_DO(ret != -1, delete tp);
    puts("threadpool create success...");

    ep = new Epoll();
    ret = ep->create(EPOLL_QUE);
    ASSERT_DO(ret != -1, delete ep);
    puts("epoll create success");

    puts("\033[01;32mrunning\033[0m...");
    status = START;
    ep->add_event(servfd, EPOLLIN);
    ep->add_event(0, EPOLLIN);  //stdin
    while(status) {
        int ret = ep->wait();

        for(int i = 0; i < ret; i++) {
            epoll_event *event = ep->get_event(i);
            if(event->data.fd == servfd) {
                Accept();
            } else if(event->data.fd == 0) {
                Handle_stdin();
            } else if(event->events & EPOLLIN){
                client clie;
                mmap_mutex.lock();
                clie.fd = event->data.fd;
                clie.addr = mmap[clie.fd];
                mmap_mutex.unlock();
                tp->add_task(Read, &clie);
            }
        }
    }
    Over();
    return 0;
}

void Accept() {
    printf("Accept() thread is %d\n", gettid());
    sockaddr_in clieaddr;
    socklen_t clieaddr_len;
    int cliefd = accept(servfd, (sockaddr*)&clieaddr, &clieaddr_len);
    printf("a new client %s:%d connect.\n", inet_ntoa(clieaddr.sin_addr), ntohs(clieaddr.sin_port));
    ep->add_event(cliefd, EPOLLIN);
    mmap[cliefd] = clieaddr;
}

void Handle_stdin() {
    printf("Handle_stdin() thread is %d\n", gettid());
    char buf[BUFFER_SIZE];
    int len = read(0, buf, BUFFER_SIZE-1);
    if(len <= 0) return;
    buf[len - 1] = '\0';
    for(int i = 0; i < len; i++) {
        if('a' <= buf[i] && buf[i] <= 'z') buf[i] -= 32;
    }
    if(strncmp("Q", buf, 2) == 0) {
        Over();
    } else if(strncmp("Quit", buf, 5) == 0) {
        Over();
    }
}

void* Read(void *args) {
    printf("Read() thread is %d\n", gettid());
    client *clie = (client*)args;
    char buf[BUFFER_SIZE];
    int len = read(clie->fd, buf, BUFFER_SIZE-1);
    if(len < 0) return nullptr;
    if(len == 0) {
        mmap_mutex.lock();
        mmap.erase(mmap.find(clie->fd));
        mmap_mutex.unlock();
        return nullptr;
    }
    buf[len] = '\0';
    printf("recv form %s:%d a msg: %s", inet_ntoa(clie->addr.sin_addr), ntohs(clie->addr.sin_port), buf);
    return nullptr;
}

int Socket(const char *ip, const int port) {
    servfd = socket(PF_INET, SOCK_STREAM, 0);
    ASSERT(servfd > 0);

    bzero(&servaddr, sizeof(servaddr));
    int ret = inet_aton(ip, &servaddr.sin_addr);
    ASSERT(ret != -1);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    
    int reuse = 1;
    ret = setsockopt(servfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ASSERT(ret != -1);

    return 0;
}

int Bind() {
    int ret = bind(servfd, (sockaddr*)&servaddr, sizeof(servaddr)); 
    ASSERT(ret != -1);

    return 0;
}

int Listen() {
    int ret = listen(servfd, MAX_LISTENQUE);
    ASSERT(ret != -1);

    return 0;
}

void Over() {
    
    printf("Over() thread is %d\n", gettid());
    if(servfd > 0) { close(servfd); servfd = -1; }

    if(ep) { delete ep; ep = nullptr; }

    if(tp) { delete tp; tp = nullptr; }

    status = STOP;
}