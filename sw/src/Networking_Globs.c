#include "Networking_Globs.h"
#include <stdint.h>

uint8_t host_mac_address[6];
uint8_t curr_packet_buffer[MTU];

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
