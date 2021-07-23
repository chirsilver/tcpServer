#include "threadpoll.h"
#include "tlvpacket.h"
#include <string.h>

void* foo(void *args) {
    TLVPacket *packet = (TLVPacket*)args;
    printf("this thread is run: type:%d, length:%d, data:%s", packet->type, packet->length, packet->value);
    return nullptr;
}

int main() {
    TLVPacket packet;
    packet.type = DATA;
    int len = snprintf(packet.value, DATASIZE-1, "packet value ");
    packet.length = len;
    Task tk(foo, &packet);

    Threadpoll tp;
    tp.threadpoll_init();

    for(int i = 0; i < 10; i++) {
        bzero(packet.value, sizeof(packet.value));
        int len = snprintf(packet.value, DATASIZE-1, "packet value %d", i);
        packet.length = len;
        tp.add_task(&tk);
    }
    return 0;
}