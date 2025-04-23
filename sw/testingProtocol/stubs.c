#include <stdint.h>
#include <stdio.h>
#include "stubs.h"

void ethernetTX(uint8_t* payload, uint16_t size){
    for (int i = 0; i < size; i++) {
        if (i % 16 == 0) printf("%04x  ", i);
        printf("%02x ", payload[i]);
        if ((i + 1) % 16 == 0 || i + 1 == size) printf("\n");
    }
    printf("\n\n");

    FILE* fptr = fopen("outbytes.txt", "w");
    for (int i = 0; i < size; i++) {
        if (i % 16 == 0) fprintf(fptr, "%06x  ", i);
        fprintf(fptr, "%02x ", payload[i]);
        if ((i + 1) % 16 == 0 || i + 1 == size) fprintf(fptr, "\n");
    }
    fclose(fptr);

}

void ethernetRX(uint8_t* payload, uint16_t size){
    //call mac api layer
}
