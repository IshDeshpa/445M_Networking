#include "Networking_Globs.h"
#include <stdint.h>

uint8_t host_mac_address[6];
uint8_t curr_packet_buffer[MTU];
uint8_t host_ip_address[4];
uint32_t subnet_mask;
uint32_t gateway;
