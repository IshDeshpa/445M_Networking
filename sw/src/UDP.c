#include "UDP.h"
#include "Networking_Globs.h"

#define SOURCE_PORT (0xBEEF)
#define CHECKSUM    (0x0000)

// data should point to
int UDP_TX(uint8_t *data, uint8_t size, uint16_t port){
    ASSERT(data == curr_packet_buffer);
    
    
}