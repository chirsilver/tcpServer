// #include "server.h"

// Server::Server():stat(0) {}

// Server::~Server() {
// }

// int Server::Socket(const char *ip, const int port) {
//     node.fd = socket(PF_INET, SOCK_STREAM, 0);
//     ASSERT(node.fd > 0);

//     bzero(&node.addr, sizeof(node.addr));
//     node.addr->sin_family = AF_INET;
//     node.addr->sin_port = htons(port);
//     int ret = inet_aton(ip, &node.addr->sin_addr);
//     ASSERT(ret == 1);

//     int reuse = 1;
//     ret = setsockopt(node.fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
//     ASSERT(ret != -1);

//     puts("socket create success...");
//     return 0;
// }

// int Server::Bind() {
//     int ret = bind(node.fd, (sockaddr*)node.addr, sizeof(*node.addr));
//     ASSERT(ret != -1);

//     puts("socket bind success...");
//     return 0;
// }

// int Server::Listen() {
//     int ret = listen(node.fd, MAX_LISTEN_QUE);
//     ASSERT(ret != -1);

//     fprintf(stdout, "socket listen on port %d...\n", ntohs(node.addr->sin_port));
//     return 0;
// }

// int Server::init() {
//     int ret = Socket(IPADDR, PORT);
//     ASSERT(!ret);

//     ret = Bind();
//     ASSERT(!ret);

//     ret = Listen();
//     ASSERT(!ret);

//     mmap[node.fd] = node;

//     epoll_event event;
//     event.data.fd = node.fd;
//     event.events = EPOLLIN | EPOLLET;
//     ret = epoll.epoll_init(node.fd, &event);
//     ASSERT(ret != -1);

//     ret = threadpoll.threadpoll_init();
//     ASSERT(ret != -1);

//     return 0;
// }

// void Server::start() {
//     while(stat) {
//         int ret = epoll.epoll_run();
//         handle_event(ret, epoll.get_events(), &threadpoll);
//     }
// }

// int Server::handle_event(int eventnum, epoll_event* events, Threadpoll *tp) {
//     Node node;
//     for(int i = 0; i < eventnum; i++) {
//         node.fd = events[i].data.fd;  
//         node.addr = mmap[node.fd].addr;
//         if(node.fd == this->node.fd) client_accept();
//         else if(events[i].events & EPOLLIN) {
//             tp->add_task(new Task(this->thread_recv, &node));
//         } else if(events[i].events & EPOLLOUT) {
//             tp->add_task(new Task(this->thread_send, &node));
//         }
//     }
// }

// int Server::client_accept() {
//     sockaddr_in *addr = new sockaddr_in;
//     socklen_t addrlen;
//     int cliefd = accept(node.fd, (sockaddr*)addr, &addrlen);
//     ASSERT(cliefd > 0);

//     mmap[cliefd] = {cliefd, addr};

//     epoll_event event;
//     event.data.fd = cliefd;
//     event.events = EPOLLIN | EPOLLET;
//     epoll.add_event(cliefd, &event);

//     return 0;
// }

// void* Server::thread_recv(void *args) {
//     Node *node = (Node*)args;
//     TLVPacket packet;
//     int ret = read(node->fd, &packet, DATASIZE-1);
//     if(ret <= 0) {
//         close(node->fd);
//         Node tmp = mmap[node->fd];
//         delete tmp.addr;

//         return nullptr;
//     }
//     printf("recv from %s:%d a %s: %s", inet_ntoa(node->addr->sin_addr), ntohs(node->addr->sin_port), prase(packet.type), packet.value);
    
//     epoll_event event;
//     event.data.fd = node->fd;
//     event.events = EPOLLOUT | EPOLLET;
//     mutex.lock();
//     epoll.mod_event(node->fd, &event);
//     mutex.unlock();
//     return nullptr;
// }

// void* Server::thread_send(void *args) {
//     Node *node = (Node*)args;
//     TLVPacket packet;
//     packet.type = DATA;
//     int len = snprintf(packet.value, DATASIZE-1, "this msg from server.\n");
//     packet.length = len + 1;
//     write(node->fd, &packet, packet.length+1+sizeof(int)*2);
//     printf("send to %s:%d a %s: %s", inet_ntoa(node->addr->sin_addr), ntohs(node->addr->sin_port), (packet.type == 1) ? "DATA" : "OTHER", packet.value);
    
//     epoll_event event;
//     event.data.fd = node->fd;
//     event.events = EPOLLOUT | EPOLLET;
//     mutex.lock();
//     epoll.del_event(node->fd, &event);
//     mutex.unlock();
//     return nullptr;
// }