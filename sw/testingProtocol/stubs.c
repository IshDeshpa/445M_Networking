#include <stdint.h>
#include <stdio.h>
#include "mac.h"
#include "printf.h"
#include "stubs.h"
#include "Networking_Globs.h"

void ethernetTX(uint8_t* payload, uint16_t size){
    for (int i = 0; i < size; i++) {
        if (i % 16 == 0) printf("%04x  ", i);
        printf("%02x ", payload[i]);
        if ((i + 1) % 16 == 0 || i + 1 == size) printf("\n");
    }
    printf("\n\n");

    FILE* fptr = fopen("temp/outbytes.txt", "w");
    for (int i = 0; i < size; i++) {
        if (i % 16 == 0) fprintf(fptr, "%06x  ", i);
        fprintf(fptr, "%02x ", payload[i]);
        if ((i + 1) % 16 == 0 || i + 1 == size) fprintf(fptr, "\n");
    }
    fclose(fptr);
}

uint8_t rx_buffer[MTU+50];
void ethernetRX (void){
    //call mac api layer
    FILE *fptr = fopen("temp/inbytes_raw.txt", "r");
    if (fptr == NULL) {
        printf("Error opening file\n");
    }

    // Seek to end
    fseek(fptr, 0L, SEEK_END);

    // Get size
    int sz = ftell(fptr);

    // Seek to beginning
    fseek(fptr, 0L, SEEK_SET);

    for (int i = 0; i < sz; i++) {
        uint8_t byte;
        fread(&byte, sizeof(uint8_t), 1, fptr);
        rx_buffer[i] = byte;
    }
    fclose(fptr);
    macRX(rx_buffer, 0);

}

void userRXData(uint8_t* payload, uint16_t size) {
    printf("========== Received Payload (%u bytes) ==========\n", size);
    for (uint16_t i = 0; i < size; i++) {
        if (i % 8 == 0) {
            printf("%04X: ", i);  // offset label
        }

        printf("%02X ", payload[i]);

        if ((i + 1) % 8 == 0 || i == size - 1) {
            printf("\n");
        }
    }
    printf("=================================================\n");
}
