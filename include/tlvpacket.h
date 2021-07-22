#ifndef _TLVPACKET_H_
#define _TLVPACKET_H_

#include "define.inc"

enum TLVType {
    DATA = 1, OTHER
};

struct TLVPacket {
    TLVType type;
    int length;
    char value[DATASIZE];
};

#endif