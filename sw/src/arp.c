/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "arp.h"
#include <stdint.h>

#ifdef SIM
#include "OS.h"
#endif 
/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */
#define IP_SIZE (4)
#define MAC_SIZE (6)
#define ARP_CACHE_SIZE (5)
/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */

uint32_t arpCache[ARP_CACHE_SIZE * (IP_SIZE + MAC_SIZE)];
OS
/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */

//TODO:needs to be made thread safe, cache is s shared global

errARP_t arpCacheInit(void){

    return APR_SUCCESS;
}
errARP_t arpRX(uint8_t * payload){

    return APR_SUCCESS;
}

//needs to send a braod cast message
errARP_t arpTX(uint8_t * payload){
    
    return APR_SUCCESS;
}
