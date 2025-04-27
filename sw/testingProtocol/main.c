#include <stdint.h>
#include "printf.h"
#include <stdio.h>
#include <string.h>
#include "UDP.h"
#include "ip.h"
#include "mac.h"
#include "Networking_Globs.h"
#include "stubs.h"

#define MSG(...) printf("\n\r"); printf("[%s][%d]", __FUNCTION__, __LINE__); printf(__VA_ARGS__); printf("\n\r")

#define PAYLOAD_SIZE 33
#define DESTINATION_IP 0xC0A80001 //192.0.1.168
uint32_t HOST_IP  = 0xC0A80064;  // 192.168.0.100

uint8_t testhostmac[6] = { 0x02, 0xAB, 0xCD, 0x34, 0x56, 0x78 };

uint8_t payload[PAYLOAD_SIZE] = {
    0x3A, 0xB7, 0x1F, 0x92, 0x6C, 0xD0, 0x8E, 0x15,
    0xF4, 0x2D, 0x7A, 0xE1, 0x9B, 0x43, 0xC6, 0x5E,
    0x01, 0xAD, 0x7F, 0x68, 0x94, 0xD7, 0x3C, 0x2B,
    0xEF, 0x88, 0x36, 0x10, 0x77, 0x5A, 0xC9, 0x42,
    0xE5
};

uint8_t message[] = "hi, shitter";

#define MESSAGE_SIZE sizeof(message)

//bc MTU is ip layer, so adding some for mac header
uint8_t networkBuf[MTU+50];

inline static void testTX(void){
    MSG("Starting up testing");

    memcpy(networkBuf, message, MESSAGE_SIZE);
    //ethernetTX(payload, PAYLOAD_SIZE);
    int ret = udp_tx(MESSAGE_SIZE, networkBuf, DESTINATION_IP, 0, 0x4567);
    if(ret == IP_SUCCESS){
        MSG("message sent succesfully");
    }else{
        MSG("message sent not sent"); 
    }

    MSG("oopsie poopsie packet recived");  
}

inline static void testRX(void){
    ethernetRX();
}

int main(int argc, char *argv[]){
    setHostMac(testhostmac);
    setHostIP((uint8_t*)&HOST_IP);
    //testTX();
    testRX();    

    return 0;
}
