/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "mac.h"
#include "ip.h"
#include "stubs.h"
#include "Networking_Globs.h"
#include <stdint.h>
#include "printf.h"
#include <string.h>
#include "string_lite.h"

/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */

typedef struct __attribute__((packed)) {
    uint8_t dest_mac[6];
    uint8_t src_mac[6];
    uint16_t ethertype; 
} macHeader_t;

#define HEADER_SIZE (14) 
const uint8_t mac_broadcast_addr[MAC_ADDR_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t test_MAC_ADDR[MAC_ADDR_SIZE] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };

/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */

uint8_t* LookUpMac(uint8_t ip);

static void headerToBigEndian(macHeader_t* header);
static void headerTolittleEndian(macHeader_t* header);
/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */
int macAddrComp(uint8_t* mac1, uint8_t* mac2){
    for(int i = 0; i < 6; i++){
        if(mac1[i] != mac2[i]) return 1;
    }
    return 0;
}

errMAC_t macRX(uint8_t* payload, uint16_t size){

    macHeader_t* macheader = (macHeader_t*)payload;
    headerTolittleEndian(macheader);
    if(macAddrComp(macheader->dest_mac, host_mac_address)){
        LOG("Dropped ethenet pkt bc destionation mac didnt match ours");
    }
    switch (macheader->ethertype) {
        case ETHERTYPE_IPV4:
            int ret = ip4_rx(payload + HEADER_SIZE);
            if(ret){
                LOG("Failed with ip rx");
            }
        case ETHERTYPE_ARP:
            LOG("Got a ARP pakcet, droping it");
            //TODO: send up to arp headnler
        default:
            printf("Unsupported EtherType Received: 0x%04X\n", macheader->ethertype);
    }
    return MAC_SUCCESS;
}

errMAC_t macTX(uint8_t* payload, uint16_t payloadsize, mac_EtherType_t ethertype_protocal){
    //omve the paylaod
    memmove(payload + HEADER_SIZE, payload, payloadsize);
    macHeader_t* macheader = (macHeader_t*)payload;
    
    switch (ethertype_protocal) {
        case ETHERTYPE_IPV4:
            macheader->ethertype = ETHERTYPE_IPV4;
            ipHeader_t* ipheader = (ipHeader_t*)(payload + HEADER_SIZE);
            uint8_t* destinationMAC = LookUpMac(packet_ntohl(ipheader->destinationIP));
            memcpy(macheader->dest_mac, destinationMAC, MAC_ADDR_SIZE);
            memcpy(macheader->src_mac, host_mac_address, MAC_ADDR_SIZE);
            headerToBigEndian(macheader);
            ethernetTX(payload, payloadsize + HEADER_SIZE);
            break;

        case ETHERTYPE_ARP:
            macheader->ethertype = ETHERTYPE_ARP;
            memcpy(macheader->dest_mac, mac_broadcast_addr, MAC_ADDR_SIZE);
            memcpy(macheader->src_mac, host_mac_address, MAC_ADDR_SIZE);
            headerToBigEndian(macheader);
            ethernetTX(payload, payloadsize + HEADER_SIZE);
            break;

        default:
            printf("Unsupported EtherType Attempted to be transmitted: 0x%04X\n", ethertype_protocal);
            break;
    }

    ethernetTX(payload, payloadsize);
    return MAC_SUCCESS;
}

void setHostMac(uint8_t mac[6]){
    memcpy(host_mac_address, mac, 6);
}

uint8_t* LookUpMac(uint8_t ip){
    return test_MAC_ADDR;
}

static void reverse_mac(uint8_t* addr) {
    for (int i = 0; i < 3; ++i) {
        uint8_t tmp = addr[i];
        addr[i] = addr[5 - i];
        addr[5 - i] = tmp;
    }
}

static void ntoh_macaddr(uint8_t* addr) {
    reverse_mac(addr);
}

static void hton_macaddr(uint8_t* addr) {
    reverse_mac(addr);
}

static void headerTolittleEndian(macHeader_t* header){
    header->ethertype = packet_ntohs(header->ethertype);
    ntoh_macaddr(header->dest_mac);    
    ntoh_macaddr(header->src_mac);    
}

static void headerToBigEndian(macHeader_t* header) {
    header->ethertype = packet_htons(header->ethertype);
    hton_macaddr(header->dest_mac);    
    hton_macaddr(header->src_mac);    
}


