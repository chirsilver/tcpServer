#ifndef _TLVPACKET_H_
#define _TLVPACKET_H_

#define DATASIZE 1024

#define DATA 1
#define OTHER 2
#define ERROR 3

struct TLVPacket {
    int type;
    int length;
    char value[DATASIZE];
};

const char *prase(int type) {
    if(type == 1) return "DATA";
    if(type == 2) return "OTHER";
    else return "ERROR";
}

#endif