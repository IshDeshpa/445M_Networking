#ifndef PING_H
#define PING_H

#include "ICMP.h"
#define MAX_PING_PACKET_SIZE 64

typedef struct {
    icmpHeader_t hdr;
    uint8_t data[MAX_PING_PACKET_SIZE];
    uint16_t data_size;
} ping_msg_t;

#endif
