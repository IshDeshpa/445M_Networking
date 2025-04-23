#ifndef MAC_H 
#define MAC_H

/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include <stdint.h>
/* ================================================== */
/*            GLOBAL VARIABLE DECLARATIONS            */
/* ================================================== */

typedef enum{
    MAC_SUCCESS,
    MAC_RX_FAIL,
    MAC_RX_FAIL_INCORRECT_DEST_MAC,
    MAC_TX_FAIL,

}errMAC_t;
/* ================================================== */
/*                 FUNCTION PROTOTYPES                */
/* ================================================== */

errMAC_t macRX(uint8_t* payload, uint16_t payloadsize);
errMAC_t macTX(uint8_t* payload, uint16_t payloadsize);
void setHostMac(uint8_t mac[6]);
#endif 

