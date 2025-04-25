#ifndef UDP_H
#define UDP_H

#include <stdint.h>
typedef enum {
    UDP_SUCCESS,
    UDP_TX_FAIL,
    UDP_RX_FAIL,
}errUDP_t;

errUDP_t udp_tx(uint8_t payloadsize, uint8_t *payload, uint32_t destinationIP, uint16_t port);
errUDP_t udp_rx(uint8_t* payload, uint16_t payloadsize);
#endif
