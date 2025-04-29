/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "arp.h"
#include "Networking_Globs.h"
#include <stdint.h>
#include "string_lite.h"
#ifdef SIM
#include "OS.h"
#endif 
/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */
// ARP constants
#define ARP_HTYPE_ETHERNET    0x0001
#define ARP_PTYPE_IPV4        0x0800
#define ARP_HLEN_ETHERNET     6
#define ARP_PLEN_IPV4         4
#define ARP_OPCODE_REQUEST    1
#define ARP_OPCODE_REPLY      2

// Macro to initialize an ARP header (defaults only)
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

typedef struct __attribute__((__packed__)){
    uint32_t TargetIp;
    uint8_t TargetMAC[6];
}arpReqLog_t;

#define ARPREQ_DATA_FIFO_SIZE (3)
OS_FIFO_t arpReqFifo;
sema4_t arpReqFifo_sema4s[3];
uint8_t arpReqFifo_buffer[ARPREQ_DATA_FIFO_SIZE * sizeof(arpReqLog_t)];

uint8_t arpBuf[MTU/2];
/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */

void print_arp_header(const arp_header_t *arp);
static void HeaderToLittleEndian(arp_header_t* hdr);
static void HeaderToBigEndian(arp_header_t* hdr);

#define INIT_ARP_HEADER_DEFAULTS(arp_ptr)   \
    do {                                    \
        (arp_ptr)->htype = (ARP_HTYPE_ETHERNET);   \
        (arp_ptr)->ptype = (ARP_PTYPE_IPV4);       \
        (arp_ptr)->hlen = ARP_HLEN_ETHERNET;            \
        (arp_ptr)->plen = ARP_PLEN_IPV4;                \
    } while (0)
/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */
errARP_t arpRX(uint8_t * payload, uint16_t payloadsize){
    arp_header_t* header = (arp_header_t*)payload;
    HeaderToLittleEndian(header);
    print_arp_header(header);
    
    if(header->opcode == ARP_OPCODE_REQUEST){
        arpReqLog_t log;
        log.TargetIp = header->sender_ip;
        memcpy(log.TargetMAC, header->sender_mac, 6);
        OS_Fifo_Put((uint8_t *)&log, &arpReqFifo);
    }else{
        LOG("Unknown arp opcode found, %u", header->opcode);
    }

    return APR_SUCCESS;
}

//needs to send a braod cast message
errARP_t arpTX(uint8_t* payload, int32_t targetIp, uint8_t* targetMAC){

    arp_header_t* hdr = (arp_header_t*)payload;
    INIT_ARP_HEADER_DEFAULTS(hdr);
    hdr->sender_ip = host_ip_address;
    memcpy(hdr->sender_mac, host_mac_address, 6);
    hdr->target_ip = targetIp;
    memcpy(hdr->sender_mac, targetMAC, 6);
    HeaderToBigEndian(hdr);

    return APR_SUCCESS;
}

void Task_ARP_RESP(void){
    LOG("Waiting for arp reqs");
    arpReqLog_t rxBuf;
    while(OS_Fifo_Get((uint8_t *)&rxBuf, &arpReqFifo)){
        LOG("arp req recived, repsing now");
        arpTX((uint8_t*)&arpBuf, rxBuf.TargetIp, rxBuf.TargetMAC);
    }
}

void arp_init(void){
    OS_Fifo_Init(ARPREQ_DATA_FIFO_SIZE, &arpReqFifo, arpReqFifo_buffer, sizeof(arpReqLog_t), arpReqFifo_sema4s);
    OS_AddThread(Task_ARP_RESP, STACKSIZE, 4);
}

void print_arp_header(const arp_header_t *arp) {
    printf("=== ARP Header ===\n");
    printf("Hardware Type: 0x%04x\n", (arp->htype));
    printf("Protocol Type: 0x%04x\n", (arp->ptype));
    printf("Hardware Size: %u\n", arp->hlen);
    printf("Protocol Size: %u\n", arp->plen);
    printf("Opcode: 0x%04x (%s)\n", (arp->opcode),
           ((arp->opcode) == 1) ? "Request" : 
           ((arp->opcode) == 2) ? "Reply" : "Unknown");

    printf("Sender MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           arp->sender_mac[0], arp->sender_mac[1], arp->sender_mac[2],
           arp->sender_mac[3], arp->sender_mac[4], arp->sender_mac[5]);

    printf("Sender IP: %u.%u.%u.%u\n",
           (arp->sender_ip >> 24) & 0xFF, (arp->sender_ip >> 16) & 0xFF,
           (arp->sender_ip >> 8) & 0xFF, arp->sender_ip & 0xFF);

    printf("Target MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           arp->target_mac[0], arp->target_mac[1], arp->target_mac[2],
           arp->target_mac[3], arp->target_mac[4], arp->target_mac[5]);

    printf("Target IP: %u.%u.%u.%u\n",
           (arp->target_ip >> 24) & 0xFF, (arp->target_ip >> 16) & 0xFF,
           (arp->target_ip >> 8) & 0xFF, arp->target_ip & 0xFF);
    printf("==================\n");
}


static void HeaderToLittleEndian(arp_header_t* hdr){
    packet_ntohs(hdr->htype);
    packet_ntohs(hdr->ptype);
    packet_ntohs(hdr->opcode);
    packet_ntohl(hdr->sender_ip);
    packet_ntohl(hdr->target_ip);
}

static void HeaderToBigEndian(arp_header_t* hdr){
    packet_htons(hdr->htype);
    packet_htons(hdr->ptype);
    packet_htons(hdr->opcode);
    packet_htonl(hdr->sender_ip);
    packet_htonl(hdr->target_ip);
}


