#ifndef NETWORKING_H 
#define NETWORKING_H

typedef enum {
    NW_SCAN,
    NW_CONNECT,
    NW_DISCONNECT,
    NW_GET_MAC,
    NW_SEND_RAW,
    NW_RECEIVE_RAW,
    NW_RECEIVE_IRQ
} network_command_type;

typedef struct {
    network_command_type command;
    uint8_t data[96]; // Example data buffer, adjust size as needed
} network_command_t;

/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */

/* ================================================== */
/*            GLOBAL VARIABLE DECLARATIONS            */
/* ================================================== */
typedef enum {
    NETWORKING_SUCCESS,
    WIFI_INIT_FAIL,
    GET_MAC_FAIL,
    LIST_SSID_FAIL
}errNetworking_t;
/* ================================================== */
/*                 FUNCTION PROTOTYPES                */
/* ================================================== */

errNetworking_t Wifi_Init(void);
errNetworking_t get_mac(void);
errNetworking_t List_SSID(void);
void Task_TestNetworking(void);

void Network_Receive_IRQ(void);

#endif 

