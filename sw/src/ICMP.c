#include "ICMP.h"
#include "ip.h"
#include "internet_checksum.h"
#include "Networking_Globs.h"

typedef enum {
    ICMP_ECHO_REPLY = 0,
    ICMP_DEST_UNREACH = 3,
    ICMP_SOURCE_QUENCH = 4,
    ICMP_REDIRECT = 5,
    ICMP_ECHO_REQUEST = 8,
    ICMP_TIME_EXCEEDED = 11,
    ICMP_PARAMETER_PROBLEM = 12,
    ICMP_TIMESTAMP_REQUEST = 13,
    ICMP_TIMESTAMP_REPLY = 14
} icmpType_t;

typedef struct __attribute__((packed)) {
    icmpType_t type;
    uint8_t code;
    uint16_t checksum;
    uint32_t rest;
} icmpHeader_t;

static inline void headerTolittleEndian(icmpHeader_t* header){
    header->checksum = packet_ntohs(header->checksum);
    header->rest = packet_ntohl(header->rest);
}

static inline void headerToBigEndian(icmpHeader_t* header){
    header->checksum = packet_htons(header->checksum);
    header->rest = packet_htonl(header->rest);
}

static inline void icmp_print_header(const icmpHeader_t* header) {
    printf("========== ICMP HEADER (HEX + DECIMAL) ==========\n");
    printf("Type              : 0x%02X (%u)\n", header->type, header->type);
    printf("Code              : 0x%02X (%u)\n", header->code, header->code);
    printf("Checksum          : 0x%04X (%u)\n", header->checksum, header->checksum);
    printf("Rest              : 0x%08X (%u)\n", header->rest, header->rest);
    printf("===============================================\n");
}

errICMP_t icmp_tx(uint8_t *payload, uint16_t payloadsize, uint32_t destinationIP){
    icmpHeader_t *header = (icmpHeader_t *)payload;
    
    // Step 1: Convert header to big-endian
    headerToBigEndian(header);
    
    // Step 2: Generate checksum in big-endian
    header->checksum = generate_checksum(header, payloadsize);
    
    // Step 3: Send to IP layer
    int ret = ip4_tx(payloadsize + sizeof(icmpHeader_t), payload, IP_PROTOCOL_ICMP, destinationIP);
    return ret == IP_SUCCESS ? ICMP_SUCCESS : ICMP_TX_FAIL;
}

errICMP_t icmp_rx(uint8_t *payload, uint16_t payloadsize){
    icmpHeader_t *header = (icmpHeader_t *)payload;
    
    icmp_print_header(header);

    uint16_t savedCksm = header->checksum;
    header->checksum = 0;
    uint16_t computed_checksum = generate_checksum(header, payloadsize);

    if (savedCksm != computed_checksum){
      LOG("Packet Dropped: Checksum Invalid: %x, %x", savedCksm, computed_checksum);
      return ICMP_RX_FAIL;
    }

    headerTolittleEndian(header);

    // TODO: Put in fifo here
}


