#include "UDP.h"
#include "ip.h"
#include "Networking_Globs.h"
#include "string_lite.h"

#define SOURCE_PORT (0xBEEF)
#define CHECKSUM    (0x0000)

typedef struct __attribute__((packed)) {
    uint16_t sourcePort;        // Source Port
    uint16_t destinationPort;   // Destination Port
    uint16_t length;            // Length of UDP header + data
    uint16_t checksum;          // Checksum
} udpHeader_t;

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

    headerToBigEndian(&header);

    ip4_tx(payloadsize + sizeof(udpHeader_t), payload, IP_PROTOCOL_UDP, destinationIP);

    return UDP_SUCCESS;
}

void headerTolittleEndian(udpHeader_t* header){
    header->sourcePort = packet_ntohs(&header->sourcePort);
    header->destinationPort = packet_ntohs(&header->destinationPort);
    header->length = packet_ntohs(&header->length);
    header->checksum = packet_ntohs(&header->checksum);
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