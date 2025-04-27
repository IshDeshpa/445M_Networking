/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "dhcp_client.h"
#include "UDP.h"
#include "mac.h"
#include "ip.h"
#include "Networking_Globs.h"
#include "string.h"
/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */
#define DHCP_DISCOVER_SIZE (240)
#define DHCP_XID (0xDEADBEEF)
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
  .mac_address = 0x0000,  // MAC + padding (set in init)
  .cookie = MAGIC_COOKIE
};

static uint32_t pendingIP;
static uint32_t dhcp_serverIP; 

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
    packet->mac_address = packet_htons(packet->mac_address);
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
    packet->mac_address = packet_ntohs(packet->mac_address);
    packet->cookie = packet_ntohl(packet->cookie);
}
/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */
int send_discover(){
    memcpy(curr_packet_buffer, &dhcp_template_packet, sizeof(dhcp_packet_t));
    
    dhcp_packet_t *pkt = ((dhcp_packet_t*)curr_packet_buffer);
    memset(pkt->misc, 0x35010100, 4); // Option 53 (DHCP Message Type) 1 octet with DHCPDISCOVER
    
    
    packetToBigEndian(curr_packet_buffer);

    return udp_tx(sizeof(dhcp_packet_t), curr_packet_buffer, 0xFFFFFFFF, 68, 67);
}

int receive_offer(dhcp_packet_t *packet, uint16_t packet_size){
    packetTolittleEndian(packet);

    ASSERT(packet->OP == 0x02);
    ASSERT(packet->HTYPE == 0x01);
    ASSERT(packet->HLEN == 0x06);
    ASSERT(packet->XID == DHCP_XID);
    ASSERT(packet->my_ip != 0);
    ASSERT(packet->server_ip != 0);

    // Get the actual destination ip
    ipHeader_t *ip_h = (udpHeader_t *)(curr_packet_buffer + sizeof(macHeader_t));
    pendingIP = ip_h->destinationIP;
    dhcp_serverIP = ip_h->sourceIP;
}

int send_request(){
     memcpy(curr_packet_buffer, &dhcp_template_packet, sizeof(dhcp_packet_t));
    
    dhcp_packet_t *pkt = ((dhcp_packet_t*)curr_packet_buffer);
    memset(pkt->misc, 0x35010100, 4); // Option 53 (DHCP Message Type) 1 octet with DHCPDISCOVER
    
    
    packetToBigEndian(curr_packet_buffer);

    return udp_tx(sizeof(dhcp_packet_t), curr_packet_buffer, dhcp_serverIP, 68, 67);
}

int receive_ack(dhcp_packet_t *packet, uint16_t packet_size){
    packetTolittleEndian(packet);

    ASSERT(packet->OP == 0x02);
    ASSERT(packet->HTYPE == 0x01);
    ASSERT(packet->HLEN == 0x06);
    ASSERT(packet->XID == DHCP_XID);
    ASSERT(packet->my_ip == pendingIP);
    ASSERT(packet->server_ip == dhcp_serverIP);

    setHostIP(&pendingIP);
}


