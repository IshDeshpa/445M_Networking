/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "ip.h"
#include <stdint.h>
#include "string_lite.h"
#include "Networking.h"
#include "string_lite.h"
#include "Networking_Globs.h"

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

#define MTU_DEFAULT (1500)


/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */

uint16_t identification = 0x1234;

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

uint16_t generate_ip4_checksum(ipHeader_t* header, uint16_t headersize);


uint16_t packet_ntohs(uint16_t* network_short);
uint32_t packet_ntohl(uint32_t* network_long);
uint16_t packet_htons(uint16_t* host_short);
uint32_t packet_htonl(uint32_t* host_long);

void headerToBigEndian(ipHeader_t* header);
void headerTolittleEndian(ipHeader_t* header);

int dropPkt(ipHeader_t* header);
/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */
errIP_t ip4_tx(uint16_t payloadsize, uint8_t* payload, uint8_t protocol, uint32_t destinationIP) {
    ipHeader_t header;

    // Step 1: Populate fields in little-endian (host order)
    header.version_ihl = ((IHL_DEFAULT << 4) & 0xF0) | (VERSION_DEFAULT & 0x0F);
    header.DSCP_ECN = ((ECN_DEFAULT << 6) & 0xC0) | (DSCP_DEFAULT & 0x3F);
    header.totalPacketLength = payloadsize + HEADER_SIZE_DEFAULT;
    header.identification = identification++;
    header.flags_fragmentOffset = FRAG_DEFAULT;
    header.TTL_protocol = ((protocol << 4) & 0xF0) | (TTL_DEFAULT & 0x0F);
    header.headerChecksum = 0; // must be 0 before computing
    header.sourceIP = *(uint32_t*)host_ip_address;
    header.destinationIP = destinationIP;

    // Step 2: Generate checksum in little-endian
    header.headerChecksum = generate_ip4_checksum(&header, HEADER_SIZE_DEFAULT);

    // Step 3: Convert all 16/32-bit fields to big-endian
    headerToBigEndian(&header);

    // Step 4: Move payload to make room for header
    memmove(payload + HEADER_SIZE_DEFAULT, payload, payloadsize);

    // Step 5: Copy header into front of buffer
    memcpy(payload, &header, HEADER_SIZE_DEFAULT);

    return NETWORKING_SUCCESS; // or whatever your success enum is
}

errIP_t ip4_rx(uint8_t* payload, uint16_t payloadsize){
    ipHeader_t* header = (ipHeader_t*)payload;
    headerTolittleEndian(&header);

    //checksum;
    uint16_t savedCksm = header->headerChecksum;
    header->headerChecksum = 0;
    uint16_t computed_checksum = generate_ip4_checksum(&header, HEADER_SIZE_DEFAULT);
    if(savedCksm != computed_checksum){
        LOG("Packet Dropped");
        return IP_RX_FAIL;
    }

    if(dropPkt(&header)){
        LOG("Packet Dropped");
        return IP_RX_FAIL;
    }


}

int dropPkt(ipHeader_t* header){
    uint8_t version = header->version_ihl & 0x0F; //pull out version
    if(version != VERSION_DEFAULT){
        LOG("Got a invalid version");
    }
}
uint16_t generate_ip4_checksum(ipHeader_t* header, uint16_t headersize) {
    uint32_t sum = 0;
    uint16_t* data = (uint16_t*)header;
    for (uint16_t i = 0; i < headersize / 2; i++) {
        sum += data[i]; // Convert each 16-bit word from network to host byte order
        if (sum > 0xFFFF) {
            sum = (sum & 0xFFFF) + 1; // Wrap around carry
        }
    }
    return ~((uint16_t)sum); // Final ones' complement
}

void headerTolittleEndian(ipHeader_t* header){
    packet_ntohs(&(header->totalPacketLength));
    packet_ntohs(&(header->identification));
    packet_ntohs(&(header->flags_fragmentOffset));
    packet_ntohl(&(header->sourceIP)); 
    packet_ntohl(&(header->destinationIP)); 
}

void headerToBigEndian(ipHeader_t* header){
    packet_htons(&(header->totalPacketLength));
    packet_htons(&(header->identification));
    packet_htons(&(header->flags_fragmentOffset));
    packet_htonl(&(header->sourceIP)); 
    packet_htonl(&(header->destinationIP));
}
uint16_t packet_ntohs(uint16_t* network_short){
    __builtin_bswap16(*network_short);
}

uint32_t packet_ntohl(uint32_t* network_long){
    __builtin_bswap32(*network_long);
}

uint16_t packet_htons(uint16_t* host_short){
    __builtin_bswap16(*host_short);

}

uint32_t packet_htonl(uint32_t* host_long){
    __builtin_bswap32(*host_long);
}
