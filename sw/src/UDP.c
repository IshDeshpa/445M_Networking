#include "UDP.h"
#include "ip.h"
#include "Networking_Globs.h"
#include <string.h>

#define SOURCE_PORT (0xBEEF)
#define CHECKSUM    (0x0000)
#define HEADER_SIZE (8)

typedef struct __attribute__((packed)) {
    uint16_t sourcePort;        // Source Port
    uint16_t destinationPort;   // Destination Port
    uint16_t length;            // Length of UDP header + data
    uint16_t checksum;          // Checksum
} udpHeader_t;


void udp_print_header(const udpHeader_t* header);

static void headerTolittleEndian(udpHeader_t* header);
static void headerToBigEndian(udpHeader_t* header);



int udp_tx(uint8_t payloadsize, uint8_t *payload, uint32_t destinationIP, uint16_t port){
    ASSERT(payload == curr_packet_buffer);
    ASSERT(payloadsize <= MTU);

    // Move for UDP header
    memmove(payload + sizeof(udpHeader_t), payload, payloadsize);

    udpHeader_t* header = (udpHeader_t*)payload;
    header->sourcePort = SOURCE_PORT;
    header->destinationPort = port;
    header->length = payloadsize + sizeof(udpHeader_t);
    ASSERT(header->length >= sizeof(udpHeader_t));
    header->checksum = CHECKSUM;

    headerToBigEndian(header);
    
    ip4_tx(payloadsize + sizeof(udpHeader_t), payload, IP_PROTOCOL_UDP, destinationIP);
    udp_print_header(header);
    return UDP_SUCCESS;
}

errUDP_t udp_rx(uint8_t* payload, uint16_t payloadsize){
    udpHeader_t* header = (udpHeader_t*)payload;

    headerTolittleEndian(header);

    uint16_t *curr_word = (uint16_t*)header;
    uint16_t checksum = 0;
    for (uint16_t i = 0; i < payloadsize / 2; i++) {
        checksum += curr_word[i];
    }

    if(checksum != 0xFFFF){
        LOG("Packet Dropped");
        return UDP_RX_FAIL;
    }


}

void udp_print_header(const udpHeader_t* header) {
    printf("========== UDP HEADER ==========\n");

    printf("Source Port        : 0x%04X (%u)\n", header->sourcePort, header->sourcePort);
    printf("Destination Port   : 0x%04X (%u)\n", header->destinationPort, header->destinationPort);
    printf("Length             : 0x%04X (%u)\n", header->length, header->length);
    printf("Checksum           : 0x%04X (%u)\n", header->checksum, header->checksum);

    printf("================================\n");
}

static void headerTolittleEndian(udpHeader_t* header){
    header->sourcePort = packet_ntohs(header->sourcePort);
    header->destinationPort = packet_ntohs(header->destinationPort);
    header->length = packet_ntohs(header->length);
    header->checksum = packet_ntohs(header->checksum);
}

static void headerToBigEndian(udpHeader_t* header) {
    header->sourcePort      = packet_htons(header->sourcePort);
    header->destinationPort = packet_htons(header->destinationPort);
    header->length          = packet_htons(header->length);
    header->checksum        = packet_htons(header->checksum);
}
