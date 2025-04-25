#ifndef ICMP_H
#define ICMP_H

#include <stdint.h>
typedef enum{
    ICMP_SUCCESS,
    ICMP_TX_FAIL,
    ICMP_RX_FAIL
} errICMP_t;

errICMP_t icmp_tx(uint8_t *payload, uint16_t payloadsize, uint32_t destinationIP);

errICMP_t icmp_rx(uint8_t *payload, uint16_t payloadsize);

#endif
