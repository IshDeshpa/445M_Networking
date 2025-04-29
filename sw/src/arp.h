#ifndef ARP_H
#define ARP_H
/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */

/* ================================================== */
/*            GLOBAL VARIABLE DECLARATIONS            */
/* ================================================== */
typedef enum{
    APR_SUCCESS,
    APR_FAILED_BROADCAST,
    APR_FAILED_RESOLVE,
    APR_FAILED_CACHE_INIT
} errARP_t;
/* ================================================== */
/*                 FUNCTION PROTOTYPES                */
/* ================================================== */
errARP_t arpRX(uint8_t * payload, uint16_t payloadsize);
void arp_init(void);

#endif 

