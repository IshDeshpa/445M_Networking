#include "ICMP.h"
#include "ip.h"
#include "internet_checksum.h"
#include "Networking_Globs.h"
#include "string.h"
#include <stdint.h>

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


char echoresp[]= "ping echo response";

errICMP_t icmp_tx(uint8_t *payload, uint16_t payloadsize, uint32_t destinationIP, icmpType_t type, uint8_t code, uint32_t rest){
    icmpHeader_t *header = (icmpHeader_t *)payload;
   
    memmove(payload + sizeof(icmpHeader_t), payload, payloadsize);

    header->type = type;
    header->code = code;
    header->rest = rest;

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

    headerTolittleEndian(header);

    if (savedCksm != computed_checksum){
        LOG("Packet Dropped: Checksum Invalid: %x, %x", savedCksm, computed_checksum);
        return ICMP_RX_FAIL;
    }

    if(header->code != 0){
        LOG("Packet Dropped: code is not servable: %u", header->code);
        return ICMP_PKT_DROPPED;
    }

    switch (header->type) {
        case ICMP_ECHO_REQUEST:
            LOG("revieced a echo req, seni g out a resp now ");
            ipHeader_t* iphdr = (ipHeader_t*)(header + IP_HEADER_SIZE);
            //icmp_tx(echoresp, sizeof(echoresp), iphdr->destinationIP, ICMP_ECHO_REPLY, 0, 0);
            break;
        default:
            LOG("Packet Dropped: type is not servable: %u", header->type);
            return ICMP_PKT_DROPPED;            
            break;
    }


    // TODO: Put in fifo here for ping taks when
    // we intergate witht the os
}


