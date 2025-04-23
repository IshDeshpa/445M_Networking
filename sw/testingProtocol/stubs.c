#include <stdint.h>
#include <stdio.h>
#include "stubs.h"
#include "Networking_Globs.h"

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

uint8_t rx_buffer[MTU+50];
uint8_t *ethernetRX(){
    //call mac api layer
    FILE *fptr = fopen("outbytes_raw.txt", "r");
    if (fptr == NULL) {
        printf("Error opening file\n");
        return;
    }

    // Seek to end
    fseek(fptr, 0L, SEEK_END);

    // Get size
    int sz = ftell(fptr);

    // Seek to beginning
    fseek(fptr, 0L, SEEK_SET);

    for (int i = 0; i < sz; i++) {
        if (i % 16 == 0) printf("%04x  ", i);
        uint8_t byte;
        fread(&byte, sizeof(uint8_t), 1, fptr);
        rx_buffer[i] = byte;
        printf("%02x ", byte);
        if ((i + 1) % 16 == 0 || i + 1 == sz) printf("\n\r");
    }
    fclose(fptr);

    return rx_buffer;
}
