#ifndef _TLVPACKET_H_
#define _TLVPACKET_H_

#define DATASIZE 1024
#define DATA 1
#define ERROR 2
#define OTHER 3

struct TLVPacket {
    int type;
    int length;
    char value[DATASIZE];
};

#endif