/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "ip.h"
#include <stdint.h>
#include "string_lite.h"
#include ""

/* ================================================== */
/*            DEFAULTS                                */           
/* ================================================== */
#define VERSION_DEFAULT (4)
#define IHL_DEFAULT (5) 

//tos defualts
#define DSCP_DEFAULT (0)
#define ECN_DEFAULT (0)

#define FRAG_DEFAULT (0x4000)//reserved:0, DF(dont frag):1, MF(more fragement): 0, frag offeset:0

#define TTL_DEFAULT (64)

#define HEADER_SIZE_DEFAULT (20) //shoud be 20 bytes

/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */

uint16_t indentfication = 0x1234;

typedef enum {
    IP_SUCCESS,
    IP_TX_FAIL,
    IP_RX_FAIL,
}errIP_t;

typedef struct __attribute__((packed)) {
    uint8_t version_ihl;             // version:bits [0:3], ihl: [4:7]
    uint8_t DSCP_ECN;                // Differentiated Services Code Point [0:5], ECN [6:7]
    uint16_t totalPacketLength;      // Total Length

    uint16_t identification;         // Identification
    uint16_t flags_fragmentOffset;   // Flags (3 bits) + Fragment Offset (13 bits)

    uint8_t TTL_protocol;            // Time To Live [0:3], protocol [4:7]
    uint16_t headerChecksum;         // Header Checksum

    uint32_t sourceIP;               // Source IP Address
    uint32_t destinationIP;          // Destination IP Address
} ipHeader_t;

/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */

uint16_t generate_ip4_checksum(uint8_t* IP4pkt_populated, ipHeader_t* header);

uint16_t ntohs(uint16_t data);

uint16_t htons(uint16_t data);
/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */
errIP_t ip4_tx(uint16_t payloadsize, uint8_t* payload, uint8_t protcol, uint32_t destinationIP){
    ipHeader_t header;

    header.version_ihl = ((IHL_DEFAULT << 4) & 0xF0) | (VERSION_DEFAULT & 0xF);
    
    header.DSCP_ECN = ((ECN_DEFAULT << 6) & 0xFC) | (DSCP_DEFAULT & 0x03);

    header.totalPacketLength = payloadsize + HEADER_SIZE_DEFAULT;

    header.flags_fragmentOffset = FRAG_DEFAULT;

    header.TTL_protocol = TTL_DEFAULT;

    //header.sourceIP = //TODO: use the defalt ip from the hdear
     
    header.destinationIP = destinationIP;
    memmove(payload + HEADER_SIZE_DEFAULT, payload, payloadsize);

    memcpy(payload, &header,  HEADER_SIZE_DEFAULT);
    //checksum shoudl be gierneated after the hdear is fully popluated 
    header.headerChecksum = generate_ip4_checksum(payload, (ipHeader_t*)payload);
}

uint16_t generate_ip4_checksum(uint8_t* IP4pkt_populated, ipHeader_t* header){
    
}

uint16_t packet_ntohs(uint16_t network_short){
    __builtin_bswap16(network_short);
}

uint32_t packet_ntohl(uint32_t network_long){
    __builtin_bswap32(network_long);
}

uint16_t packet_htons(uint16_t host_short){
    __builtin_bswap16(host_short);

}

uint32_t packet_htonl(uint32_t host_long){
    __builtin_bswap32(host_long);
}
