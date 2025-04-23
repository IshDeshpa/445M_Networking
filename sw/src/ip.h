#ifndef IP_H 
#define IP_H
/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */

/* ================================================== */
/*            GLOBAL VARIABLE DECLARATIONS            */
/* ================================================== */
#define IP_PROTOCOL_ICMP   1
#define IP_PROTOCOL_IGMP   2
#define IP_PROTOCOL_TCP    6
#define IP_PROTOCOL_UDP    17
#define IP_PROTOCOL_ENCAP  41
#define IP_PROTOCOL_OSPF   89
#define IP_PROTOCOL_SCTP   132

typedef enum {
    IP_SUCCESS,
    IP_TX_FAIL,
    IP_RX_FAIL,
    IP_RX_PCKT_DROPPED,
    IP_RX_UNSUPPORTED_PROTOCOL,
}errIP_t;

/* ================================================== */
/*                 FUNCTION PROTOTYPES                */
/* ================================================== */

errIP_t ip4_rx(uint8_t* payload, uint16_t payloadsize);
errIP_t ip4_tx(uint16_t payloadsize, uint8_t* payload, uint8_t protocol, uint32_t destinationIP);
#endif 

