#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ip.h"
#include "UDP.h"
#include "Networking_Globs.h"
#include "stubs.h"

#define PAYLOAD_SIZE 33
#define DESTINATION_IP 0xC0A80001 //192.0.1.168

uint8_t payload[PAYLOAD_SIZE] = {
    0x3A, 0xB7, 0x1F, 0x92, 0x6C, 0xD0, 0x8E, 0x15,
    0xF4, 0x2D, 0x7A, 0xE1, 0x9B, 0x43, 0xC6, 0x5E,
    0x01, 0xAD, 0x7F, 0x68, 0x94, 0xD7, 0x3C, 0x2B,
    0xEF, 0x88, 0x36, 0x10, 0x77, 0x5A, 0xC9, 0x42,
    0xE5
};

//bc MTU is ip layer, so adding some for mac header
uint8_t networkBuf[MTU+50];

int main(int argc, char *argv[]){
    LOG("Starting up testing\n\r");
    memcpy(networkBuf, payload, PAYLOAD_SIZE);
    //ethernetTX(payload, PAYLOAD_SIZE);
    //int ret = ip4_tx(PAYLOAD_SIZE, networkBuf, IP_PROTOCOL_UDP, DESTINATION_IP);
    int ret = udp_tx(PAYLOAD_SIZE, payload, DESTINATION_IP, 4567);
    if(ret == IP_SUCCESS){
        printf("Ip sent succesfully\n");
    }else{
        printf("Ip sent not sent\n"); 
    }
    printf("\n");

    return 0;
}
