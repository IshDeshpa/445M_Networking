#ifndef IP_H 
#define IP_H
/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include <stdint.h>
/* ================================================== */
/*            GLOBAL VARIABLE DECLARATIONS            */
/* ================================================== */
typedef enum {
    IP_PROTOCOL_ICMP     = 1,    // Internet Control Message Protocol
    IP_PROTOCOL_IGMP     = 2,    // Internet Group Management Protocol
    IP_PROTOCOL_TCP      = 6,    // Transmission Control Protocol
    IP_PROTOCOL_UDP      = 17,   // User Datagram Protocol
    IP_PROTOCOL_IPV6     = 41,   // IPv6 encapsulation
    IP_PROTOCOL_GRE      = 47,   // Generic Routing Encapsulation
    IP_PROTOCOL_ESP      = 50,   // Encrypted Security Payload (IPsec)
    IP_PROTOCOL_AH       = 51,   // Authentication Header (IPsec)
    IP_PROTOCOL_ICMPV6   = 58,   // ICMP for IPv6
    IP_PROTOCOL_OSPF     = 89,   // Open Shortest Path First
    IP_PROTOCOL_SCTP     = 132   // Stream Control Transmission Protocol
} IpProtocol_t;

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

errIP_t ip4_rx(uint8_t* payload);
errIP_t ip4_tx(uint16_t payloadsize, uint8_t* payload, IpProtocol_t protocol, uint32_t destinationIP);
void ip4_print_header(ipHeader_t* header);
void setHostIP(uint8_t ip_address[4]);
#endif 

