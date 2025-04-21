#ifndef NETWORKING_GLOBS_H
#define NETWORKING_GLOBS_H

#define MTU (1500)

#define LOG(...) printf("[%s][%d]", __FUNCTION__, __LINE__); printf(__VA_ARGS__); printf("\n\r")
#define ASSERT(x) do {if(!x){ LOG("ASSERT FAILED: %s", x); while(1){} }} while(0)

extern uint8_t host_ip_address[4];
extern uint8_t host_mac_address[6];
extern uint8_t curr_packet_buffer[MTU];

#endif