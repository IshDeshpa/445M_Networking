/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "dhcp_client.h"
#include "UDP.h"
#include "mac.h"
#include "ip.h"
#include "Networking_Globs.h"
#include "string.h"
#include <stdint.h>
/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */
#define DHCP_DISCOVER_SIZE (240)
#define DHCP_XID (0xDEADBEEF)

uint8_t dhcp_tx_buf[MTU];

static dhcp_packet_t dhcp_template_packet = {
  .OP = 0x01, // OP (request)
  .HTYPE = 0x01, // HTYPE (ethernet; should this be wifi (IEEE 802)?)
  .HLEN = 0x06, // HLEN (IEEE 802 mac addresses have hardware address length of 6)
  .HOPS = 0x00, // HOPS (for request relaying, # of hops) 
  .XID = DHCP_XID, // XID (random 32 bit number)
  .secs = 0x0000, // Secs
  .flags = 0x0000, // Flags
  .client_ip = 0x00000000, // Client IP address
  .my_ip = 0x00000000, // My IP address (unknown)
  .server_ip = 0x00000000, // Server IP address (unknown)
  .gateway_ip = 0x00000000, // Gateway IP address (unknown)
  .cookie = MAGIC_COOKIE
};

static uint32_t pendingIP;
static uint32_t dhcp_serverIP;
static uint8_t dhcp_serverMac[6];

/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */
static inline void packetToBigEndian(dhcp_packet_t *packet){
    packet->XID = packet_htonl(packet->XID);
    packet->secs = packet_htons(packet->secs);
    packet->flags = packet_htons(packet->flags);
    packet->client_ip = packet_htonl(packet->client_ip);
    packet->my_ip = packet_htonl(packet->my_ip);
    packet->server_ip = packet_htonl(packet->server_ip);
    packet->gateway_ip = packet_htonl(packet->gateway_ip);
    reverse_mac(packet->mac_address);
    packet->cookie = packet_htonl(packet->cookie);
}

static inline void packetTolittleEndian(dhcp_packet_t *packet){
    packet->XID = packet_ntohl(packet->XID);
    packet->secs = packet_ntohs(packet->secs);
    packet->flags = packet_ntohs(packet->flags);
    packet->client_ip = packet_ntohl(packet->client_ip);
    packet->my_ip = packet_ntohl(packet->my_ip);
    packet->server_ip = packet_ntohl(packet->server_ip);
    packet->gateway_ip = packet_ntohl(packet->gateway_ip);
    reverse_mac(packet->mac_address);
    packet->cookie = packet_ntohl(packet->cookie);
}
/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */
static void dhcp_print_packet(const dhcp_packet_t *pkt) {
    if (!pkt) {
        printf("Null DHCP packet\n");
        return;
    }

    printf("DHCP Packet:\n");
    printf("  OP:           0x%02X\n", pkt->OP);
    printf("  HTYPE:        0x%02X\n", pkt->HTYPE);
    printf("  HLEN:         %u\n", pkt->HLEN);
    printf("  HOPS:         %u\n", pkt->HOPS);
    printf("  XID:          0x%08X\n", pkt->XID);
    printf("  SECS:         %u\n", pkt->secs);
    printf("  FLAGS:        0x%04X\n", pkt->flags);

    printf("  Client IP:    %u.%u.%u.%u\n",
        (pkt->client_ip >> 24) & 0xFF,
        (pkt->client_ip >> 16) & 0xFF,
        (pkt->client_ip >>  8) & 0xFF,
        (pkt->client_ip >>  0) & 0xFF);

    printf("  Your IP:      %u.%u.%u.%u\n",
        (pkt->my_ip >> 24) & 0xFF,
        (pkt->my_ip >> 16) & 0xFF,
        (pkt->my_ip >>  8) & 0xFF,
        (pkt->my_ip >>  0) & 0xFF);

    printf("  Server IP:    %u.%u.%u.%u\n",
        (pkt->server_ip >>  24) & 0xFF,
        (pkt->server_ip >>  16) & 0xFF,
        (pkt->server_ip >>  8) & 0xFF,
        (pkt->server_ip >>  0) & 0xFF);

    printf("  Gateway IP:   %u.%u.%u.%u\n",
        (pkt->gateway_ip >>  24) & 0xFF,
        (pkt->gateway_ip >>  16) & 0xFF,
        (pkt->gateway_ip >> 8) & 0xFF,
        (pkt->gateway_ip >> 0) & 0xFF);
    
    printf("CLient MAC     : %02X:%02X:%02X:%02X:%02X:%02X\n",
          pkt->mac_address[5], pkt->mac_address[4], pkt->mac_address[3],
           pkt->mac_address[2], pkt->mac_address[1], pkt->mac_address[0]);

    //printf("  MAC Address:  0x%04X\n", pkt->mac_address);

    printf("  Magic Cookie: 0x%08X\n", pkt->cookie);

    printf("  Options (first 16 bytes): ");
    for (int i = 0; i < 16; i++) {
        printf("%02X ", pkt->options[i]);
    }
    printf("...\n");
}

uint32_t dhcp_discover_magicnum = 0x00350101;
int dhcp_send_discover(){
    memcpy(dhcp_tx_buf, &dhcp_template_packet, sizeof(dhcp_packet_t));
    
    dhcp_packet_t *pkt = ((dhcp_packet_t*)dhcp_tx_buf);
    
    memset(pkt->mac_address, 0, 16);
    memcpy(pkt->mac_address, host_mac_address, 6);
  
    //reverse_mac(pkt->mac_address);

    //memset(pkt->options, 0xFF, sizeof(pkt->options));

    memcpy(pkt->options, &dhcp_discover_magicnum, 3); // Option 53 (DHCP Message Type) 1 octet with DHCPDISCOVER 
  
    dhcp_print_packet(pkt);

    uint8_t temp = *pkt->options;
    *pkt->options = pkt->options[2];
    pkt->options[2] = temp;
    
    pkt->options[3] = 0xFF;
    
    packetToBigEndian(pkt);
    
    printf("%d\n\r", sizeof(dhcp_packet_t));
    return udp_tx(sizeof(dhcp_packet_t), dhcp_tx_buf, 0xFFFFFFFF, 68, 67);
}
 
int dhcp_receive_offer(uint8_t *pkt, uint16_t packet_size){
    dhcp_packet_t *packet = (dhcp_packet_t*)pkt;

    packetTolittleEndian(packet);

    dhcp_print_packet(packet);

    ASSERT(packet->OP == 0x02);
    ASSERT(packet->HTYPE == 0x01);
    ASSERT(packet->HLEN == 0x06);
    ASSERT(packet->XID == DHCP_XID);
    ASSERT(packet->my_ip != 0);
    ASSERT(packet->server_ip != 0);

    // Get the actual destination ip
    macHeader_t *mac_h = (macHeader_t *)(dhcp_tx_buf);

    ASSERT(macAddrComp(host_mac_address, mac_h->dest_mac));
    
    pendingIP = packet->my_ip;
    dhcp_serverIP = packet->server_ip;
    //LOG("\n\n\n\n\n\nserverIP set: %08X", dhcp_serverIP);
    //memcpy(dhcp_serverIP,  &ip_h->sourceIP, 4);
    
    return 0;
}

uint32_t dhcp_discover_magicnum2 = 0x00350103;
int dhcp_send_request(){
    memcpy(dhcp_tx_buf, &dhcp_template_packet, sizeof(dhcp_packet_t));

    dhcp_packet_t *pkt = ((dhcp_packet_t*)dhcp_tx_buf);
    
    memset(pkt->mac_address, 0, 16);
    memcpy(pkt->mac_address, host_mac_address, 6);
    //reverse_mac(pkt->mac_address);
    pkt->OP = 1;
    pkt->server_ip = dhcp_serverIP;
   
    //memset(pkt->options, 0x350103, 3);
    //memset(&pkt->options[3], pendingIP, 4);
    //memset(&pkt->options[7], dhcp_serverIP, 4);
    uint32_t pendIP_le = packet_htonl(pendingIP);
    uint32_t servIP_le = packet_htonl(dhcp_serverIP);

    memcpy(pkt->options, &dhcp_discover_magicnum2, 3);
    pkt->options[3] =50;
    pkt->options[4]=4;
    memcpy(&pkt->options[5], &pendIP_le, 4);
    pkt->options[9] =54;
    pkt->options[10]=4;
    memcpy(&pkt->options[11], &servIP_le, 4);
    
    dhcp_print_packet(pkt);
    
    uint8_t temp = *pkt->options;
    *pkt->options = pkt->options[2];
    pkt->options[2] = temp;

    pkt->options[15] = 0xFF;
    
    packetToBigEndian(pkt);

    return udp_tx(sizeof(dhcp_packet_t), dhcp_tx_buf, 0xFFFFFFFF, 68, 67);
}

int dhcp_receive_ack(uint8_t *pkt, uint16_t packet_size){
    dhcp_packet_t *packet = (dhcp_packet_t*)pkt;
    packetTolittleEndian(packet);

    dhcp_print_packet(packet);

    ASSERT(packet->OP == 0x02);
    ASSERT(packet->HTYPE == 0x01);
    ASSERT(packet->HLEN == 0x06);
    ASSERT(packet->XID == DHCP_XID);
    ASSERT(packet->my_ip == pendingIP);
    ASSERT(packet->server_ip == dhcp_serverIP);
    
    macHeader_t *mac_h = (macHeader_t *)(dhcp_tx_buf);
    ASSERT(mac_h->dest_mac == host_mac_address);

    setHostIP(pendingIP);

    return 0;
}

#define DHCP_OPTIONS_MAGICNUM_IDX (2)
#define DHCP_OPTIONS_MAGICNUM_OFFER_MASK (0x02)
#define DHCP_OPTIONS_MAGICNUM_ACK_MASK (0x05)

int dhcpRX(uint8_t* payload, uint16_t payloadsize){
    dhcp_packet_t* header = (dhcp_packet_t*)payload;
    if(header->options[DHCP_OPTIONS_MAGICNUM_IDX] == DHCP_OPTIONS_MAGICNUM_OFFER_MASK){
        dhcp_receive_offer(payload, payloadsize);
    }else if(header->options[DHCP_OPTIONS_MAGICNUM_IDX] == DHCP_OPTIONS_MAGICNUM_ACK_MASK){
        dhcp_receive_ack(payload, payloadsize);
    }
}

