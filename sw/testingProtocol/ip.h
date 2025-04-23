#ifndef IP_H 
#define IP_H
/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include <stdint.h>
/* ================================================== */
/*            GLOBAL VARIABLE DECLARATIONS            */
/* ================================================== */
#define IP_PROTOCOL_ICMP     1    // Internet Control Message Protocol
#define IP_PROTOCOL_IGMP     2    // Internet Group Management Protocol
#define IP_PROTOCOL_TCP      6    // Transmission Control Protocol
#define IP_PROTOCOL_UDP      17   // User Datagram Protocol
#define IP_PROTOCOL_IPV6     41   // IPv6 encapsulation
#define IP_PROTOCOL_GRE      47   // Generic Routing Encapsulation
#define IP_PROTOCOL_ESP      50   // Encrypted Security Payload (IPsec)
#define IP_PROTOCOL_AH       51   // Authentication Header (IPsec)
#define IP_PROTOCOL_ICMPV6   58   // ICMP for IPv6
#define IP_PROTOCOL_OSPF     89   // Open Shortest Path First
#define IP_PROTOCOL_SCTP     132  // Stream Control Transmission Protocol

typedef enum {
    IP_SUCCESS,
    IP_TX_FAIL,
    IP_RX_FAIL,
    IP_RX_PCKT_DROPPED,
    IP_RX_UNSUPPORTED_PROTOCOL,
}errIP_t;

typedef struct __attribute__((packed)) {
    uint8_t version_ihl;             // version:bits [0:3], ihl: [4:7]
    uint8_t DSCP_ECN;                // Differentiated Services Code Point [0:5], ECN [6:7]
    uint16_t totalPacketLength;      // Total Length

    uint16_t identification;         // Identification
    uint16_t flags_fragmentOffset;   // Flags (3 bits) + Fragment Offset (13 bits)

    uint8_t TTL;            
    uint8_t protocol;
    uint16_t headerChecksum;         // Header Checksum

    uint32_t sourceIP;               // Source IP Address
    uint32_t destinationIP;          // Destination IP Address
} ipHeader_t;

/* ================================================== */
/*                 FUNCTION PROTOTYPES                */
/* ================================================== */

errIP_t ip4_rx(uint8_t* payload, uint16_t payloadsize);
errIP_t ip4_tx(uint16_t payloadsize, uint8_t* payload, uint8_t protocol, uint32_t destinationIP);
void ip4_print_header(ipHeader_t* header);
void setHostIP(uint8_t ip_address[4]);
#endif 

