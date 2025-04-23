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


/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */

errMAC_t macRX(uint8_t* payload, uint16_t payloadsize){
    return MAC_SUCCESS;
}

errMAC_t macTX(uint8_t* payload, uint16_t payloadsize){
    ethernetTX(payload, payloadsize);
    return MAC_SUCCESS;
}

void setHostMac(uint8_t mac[6]){
    memcpy(host_mac_address, mac, 6);
}
