/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "mac.h"
#include "stubs.h"
#include "Networking_Globs.h"
#include <stdint.h>
#include <string.h>

/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */

typedef struct __attribute__((packed)) {
    uint8_t dest_mac[6];
    uint8_t src_mac[6];
    uint16_t ethertype; 
} macHeader_t;

/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */
#define HEADER_SIZE (14) 

/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */
int macAddrComp(uint8_t* mac1, uint8_t* mac2){
    for(int i = 0; i < 6; i++){
        if(dest_mac[i] != host_mac_address[i]) return 1;
    }
    return 0;
}

errMAC_t macRX(uint8_t* payload){
    macHeader_t* header = (macHeader_t*)payload;
    if(macAddrComp(header->dest_mac, host_mac_address)){
        return MAC_RX_FAIL_INCORRECT_DEST_MAC;
    }


    return MAC_SUCCESS;
}

errMAC_t macTX(uint8_t* payload, uint16_t payloadsize){
    ethernetTX(payload, payloadsize);
    return MAC_SUCCESS;
}

void setHostMac(uint8_t mac[6]){
    memcpy(host_mac_address, mac, 6);
}
