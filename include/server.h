#ifndef _SERVER_H_
#define _SERVER_H_

#include "epoll.h"
#include "threadpoll.h"
#include "tlvpacket.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <map>

#define ASSERT(exep) ((exep) ? ({void(0);}) : ({return -1;}))
#define MAX_LISTEN_QUE 10
#define MAX_BUFFER 1024
#define IPADDR "127.0.0.1"
#define PORT 8787

struct Node {
    int fd;
    sockaddr_in *addr;
};


class Server {
private:
    int stat;
    Node node; 
    Epoll epoll;
    Threadpoll threadpoll;
    std::map<int, Node> mmap;
    Mutex mutex;
public:
    Server();
    ~Server();
    int Socket(const char *ip, const int port);
    int Bind();
    int Listen();

    int init();
    void start();
    int client_accept();
    void *thread_recv(void *args);
    void *thread_send(void *args);
    int handle_event(int eventnum, epoll_event *events, Threadpoll *tp);
};

#endif