#include "UDP.h"
#include "DHCP.h"
#include "OS.h"
#include "ip.h"
#include "Networking_Globs.h"
#include "string_lite.h"
#include "stubs.h"
#include "userApp_udp.h"
#define CHECKSUM    (0x0000)
#define HEADER_SIZE (8)

void udp_print_header(const udpHeader_t* header);

static void headerTolittleEndian(udpHeader_t* header);
static void headerToBigEndian(udpHeader_t* header);



errUDP_t udp_tx(uint16_t payloadsize, uint8_t *payload, uint32_t destinationIP, uint16_t sourcePort, uint16_t destPort){
    //ASSERT(payload == dhcp_tx_buf);
    //ASSERT(payloadsize <= MTU);

    // Move for UDP header
    memmove(payload + sizeof(udpHeader_t), payload, payloadsize);

    udpHeader_t* header = (udpHeader_t*)payload;
    header->sourcePort = sourcePort;
    header->destinationPort = destPort;
    header->length = payloadsize + sizeof(udpHeader_t);
    //ASSERT(header->length >= sizeof(udpHeader_t));
    header->checksum = CHECKSUM;

    udp_print_header(header);
    headerToBigEndian(header);
    
    ip4_tx(payloadsize + sizeof(udpHeader_t), payload, IP_PROTOCOL_UDP, destinationIP);
    return UDP_SUCCESS;
}

extern OS_FIFO_t userAppFifo;
errUDP_t udp_rx(uint8_t* payload, uint16_t payloadsize){
    udpHeader_t* header = (udpHeader_t*)payload;

    headerTolittleEndian(header);
    udp_print_header(header);

    //uint16_t *curr_word = (uint16_t*)header;
    //uint16_t checksum = 0;
    //for (uint16_t i = 0; i < payloadsize / 2; i++) {
   //     checksum += curr_word[i];
   // }

    //if(checksum != 0xFFFF){
    //    LOG("Packet Dropped");
    //    return UDP_RX_FAIL;
    //}

    if(header->destinationPort == 68){
        dhcpRX(payload + sizeof(udpHeader_t), payloadsize - sizeof(udpHeader_t));
    }else if(header->destinationPort == USERAPP_UDP_PORT){//chat siad unused
        OS_Fifo_Put(payload + sizeof(udpHeader_t), &userAppFifo);
    }

    // userRXData(payload + HEADER_SIZE, (header->length)-HEADER_SIZE); 
    return UDP_SUCCESS;
}

void udp_print_header(const udpHeader_t* header) {
    printf("========== UDP HEADER ==========\n\r");

    printf("Source Port        : 0x%04X (%u)\n\r", header->sourcePort, header->sourcePort);
    printf("Destination Port   : 0x%04X (%u)\n\r", header->destinationPort, header->destinationPort);
    printf("Length             : 0x%04X (%u)\n\r", header->length, header->length);
    printf("Checksum           : 0x%04X (%u)\n\r", header->checksum, header->checksum);

    printf("================================\n\r");
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
