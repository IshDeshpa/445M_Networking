#include <stdint.h>
#include <stdio.h>
#include "stubs.h"

void ethernetTX(uint8_t* payload, uint16_t size){
    for(int i = 0; i < size; i++){
        printf("0x%02X ", payload[i]);
    }
}

void ethernetRX(uint8_t* payload, uint16_t size){
    //call mac api layer
}
