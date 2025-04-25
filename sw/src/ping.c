#include "ping.h"
#include "OS.h"
#include "ICMP.h"
#include "internet_checksum.h"
#include "string_lite.h"

#define PING_Q_SIZE 10

OS_FIFO_t ping_q;
sema4_t ping_semas[3];
ping_msg_t ping_buf[PING_Q_SIZE];

void ping_init(){
    OS_Fifo_Init(PING_Q_SIZE, &ping_q, ping_buf, sizeof(ping_msg_t), ping_semas);
}

// Register this as a callback in icmp before startup
void ping_rx(icmpHeader_t *hdr, uint8_t *data, uint16_t data_size){
    ping_msg_t msg;
    memcpy(&msg.hdr, hdr, sizeof(icmpHeader_t));
    memcpy(&msg.data, data, MAX_PING_PACKET_SIZE);
    memcpy(&msg.data_size, &data_size, sizeof(uint16_t));

    OS_Fifo_Put(&msg, &ping_q);
}

void Task_Ping(){
    ping_init();

    while(1){
        ping_msg_t data_in;
        OS_Fifo_Get(&data_in, &ping_q);

        switch(data_out.hdr.type){
            case ICMP_ECHO_REQUEST:
                data_out.hdr.type = ICMP_ECHO_REPLY;
                data_out.hdr.code = 0;
                data_out.hdr.rest = data_in.hdr.rest;
                break;
        }

         
    }
}
