/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "arp.h"
#include "Networking_Globs.h"
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

typedef struct __attribute__((__packed__)) {
    uint16_t htype;
    uint16_t ptype;
    uint8_t  hlen;
    uint8_t  plen;
    uint16_t opcode;
    uint8_t  sender_mac[6];
    uint32_t  sender_ip;
    uint8_t  target_mac[6];
    uint32_t  target_ip;
} arp_header_t;

uint32_t arpCache[ARP_CACHE_SIZE * (IP_SIZE + MAC_SIZE)];
/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */
void print_arp_header(const arp_header_t *arp);
/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */

//TODO:needs to be made thread safe, cache is s shared global

//errARP_t arpCacheInit(void){
//
//    return APR_SUCCESS;
//}

static void HeaderToLittleEndian(arp_header_t* hdr){
    packet_ntohs(hdr->htype);
    packet_ntohs(hdr->ptype);
    packet_ntohs(hdr->opcode);
}

static void HeaderToBigEndian(arp_header_t* hdr){
    packet_htons(hdr->htype);
    packet_htons(hdr->ptype);
    packet_htons(hdr->opcode);
}

errARP_t arpRX(uint8_t * payload){
    arp_header_t* header = (arp_header_t*)payload;
    HeaderToLittleEndian(header); 

    print_arp_header(header);

    return APR_SUCCESS;
}

//needs to send a braod cast message
errARP_t arpTX(uint8_t * payload){
    
    return APR_SUCCESS;
}


void print_arp_header(const arp_header_t *arp) {
    printf("=== ARP Header ===\n");
    printf("Hardware Type: 0x%04x\n", (arp->htype));
    printf("Protocol Type: 0x%04x\n", (arp->ptype));
    printf("Hardware Size: %u\n", arp->hlen);
    printf("Protocol Size: %u\n", arp->plen);
    printf("Opcode: 0x%04x (%s)\n", (arp->opcode),
           (ntohs(arp->opcode) == 1) ? "Request" : 
           (ntohs(arp->opcode) == 2) ? "Reply" : "Unknown");

    printf("Sender MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           arp->sender_mac[0], arp->sender_mac[1], arp->sender_mac[2],
           arp->sender_mac[3], arp->sender_mac[4], arp->sender_mac[5]);

    printf("Sender IP: %u.%u.%u.%u\n",
           arp->sender_ip[0], arp->sender_ip[1], arp->sender_ip[2], arp->sender_ip[3]);

    printf("Target MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           arp->target_mac[0], arp->target_mac[1], arp->target_mac[2],
           arp->target_mac[3], arp->target_mac[4], arp->target_mac[5]);

    printf("Target IP: %u.%u.%u.%u\n",
           arp->target_ip[0], arp->target_ip[1], arp->target_ip[2], arp->target_ip[3]);
    printf("==================\n");
}
