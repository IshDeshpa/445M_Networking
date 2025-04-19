/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "Networking.h"
#include <stdint.h>
#include "../lib/std/stdio_lite/stdio_lite.h"

#include "bsp/include/nm_bsp.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"
#include "driver/include/m2m_wifi.h"
#include "m2m_types.h"
#include "OS.h"
#include "tm4c123gh6pm.h"

#include "string_lite.h"

/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */
tstrWifiInitParam wifi_init_param;

/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */

#define LOG(...) printf(__VA_ARGS__); printf("\n\r")

/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */

void wifi_callback(uint8 u8MsgType, void *pvMsg) {
    LOG("Wi-Fi callback triggered! Message type: %d\n\r", u8MsgType);
}

void print_mac(uint8_t *mac) {
    printf("%02X:%02X:%02X:%02X:%02X:%02X\n\r",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

errNetworking_t Wifi_Init(void){

    // nm_bsp_init();

    LOG("Starting Up Wifi Init\n\r");

    wifi_init_param.pfAppWifiCb = &wifi_callback;
    wifi_init_param.pfAppMonCb = NULL;
    wifi_init_param.strEthInitParam.au8ethRcvBuf = NULL;
    wifi_init_param.strEthInitParam.u16ethRcvBufSize = 0;
    wifi_init_param.strEthInitParam.pfAppEthCb = NULL;

    int8_t ret = m2m_wifi_init(&wifi_init_param);
    if(ret == M2M_SUCCESS){
        LOG("Wifi Init Sucess\n\r");
        ret = NETWORKING_SUCCESS;
    }else{
       LOG("Failed with exit code: %d\n\r", ret);
        ret = WIFI_INIT_FAIL;
    }
    return ret;    
}

errNetworking_t get_mac(void) {
    uint8_t mac_ap[6];    // MAC for AP interface
    uint8_t mac_sta[6];   // MAC for STA interface
    
    int ret = NETWORKING_SUCCESS;
    if (m2m_wifi_get_mac_address(mac_ap, mac_sta) == M2M_SUCCESS) {
        LOG("AP MAC:  ");
        print_mac(mac_ap);

        LOG("STA MAC: ");
        print_mac(mac_sta);

    } else {
        LOG("Failed to get MAC addresses\n");
        ret = GET_MAC_FAIL;
    }
    return ret;
}

errNetworking_t List_SSID(void){
    int ret = NETWORKING_SUCCESS;
    LOG("Requesting Scan");
    ret = m2m_wifi_request_scan(M2M_WIFI_CH_11);
    if(ret == M2M_SUCCESS){
        LOG("Scan Request Sucessful");
    }else{
        LOG("Scan Request Failed");
        ret = LIST_SSID_FAIL;
    }
    return ret; 
}


void Task_TestNetworking(void){
    printf("\n\n\n\n\n\n\n\r");
    nm_bsp_init();
    LOG("nm_bsp_init success");

    Wifi_Init();
    get_mac();
    List_SSID();
    while(1){
        //printf("TestThread Sleeping\n\r");
        GPIO_PORTF_DATA_R ^= 0x04;
        OS_Sleep(1000);
    }
}

#define NETWORK_COMMAND_FIFO_SIZE 32
OS_FIFO_t network_command_fifo;
sema4_t network_command_sema4;
network_command_t network_command_fifo_buffer[NETWORK_COMMAND_FIFO_SIZE];

void Network_Receive_IRQ(void){
    network_command_t cmd;
    cmd.command = NW_RECEIVE_IRQ;
    memset(cmd.data, 0, sizeof(cmd.data));

    OS_Fifo_Put((uint8_t*)&cmd, &network_command_fifo);
}

void Network_Scan(void){
    network_command_t cmd;
    cmd.command = NW_SCAN;
    memset(cmd.data, 0, sizeof(cmd.data));

    OS_Fifo_Put((uint8_t*)&cmd, &network_command_fifo);
}

void Network_Connect(char *ssid, char *password){
    network_command_t cmd;
    cmd.command = NW_CONNECT;
    memset(cmd.data, 0, sizeof(cmd.data));

    strncpy((char*)cmd.data, ssid, 32);
    strncpy((char*)cmd.data + 32, password, 40);

    OS_Fifo_Put((uint8_t*)&cmd, &network_command_fifo);
}

void Network_Disconnect(void){
    network_command_t cmd;
    cmd.command = NW_DISCONNECT;
    memset(cmd.data, 0, sizeof(cmd.data));

    OS_Fifo_Put((uint8_t*)&cmd, &network_command_fifo);
}

void Network_Send_Raw(void){
    network_command_t cmd;
    cmd.command = NW_SEND_RAW;
    memset(cmd.data, 0, sizeof(cmd.data));

    OS_Fifo_Put((uint8_t*)&cmd, &network_command_fifo);
}

void Network_Receive_Raw(void){
    network_command_t cmd;
    cmd.command = NW_RECEIVE_RAW;
    memset(cmd.data, 0, sizeof(cmd.data));

    OS_Fifo_Put((uint8_t*)&cmd, &network_command_fifo);
}

void Task_NetworkThread(void){
    OS_InitSemaphore(&network_command_sema4, 1);
    OS_Fifo_Init(NETWORK_COMMAND_FIFO_SIZE, 
                &network_command_fifo, 
                (uint8_t*)network_command_fifo_buffer,
                sizeof(network_command_t),
                &network_command_sema4);

    nm_bsp_init();
    LOG("NM BSP init finished\n\r");
    Wifi_Init();
    LOG("Wifi Init finished\n\r");
    
    while(1){
        // Handle incoming commands
        network_command_t *cmd;
        OS_Fifo_Get((uint8_t*)&cmd, &network_command_fifo);
        
        sint8 res;
        switch(cmd->command){
            case NW_SCAN:
                LOG("Scan command received");
                // Call the scan function here
                res = m2m_wifi_request_scan(M2M_WIFI_CH_ALL);
                if(res == M2M_SUCCESS)
                    LOG("Scan request sent");
                break;
            case NW_CONNECT:
                LOG("Connect command received");
                // Call the connect function here
                
                // Arg parsing
                char ssid[32];
                tuniM2MWifiAuth auth_param = {0};
                
                strncpy(ssid, cmd->data, 32);
                strncpy(auth_param.au8PMK, cmd->data + 32, 40);
                LOG("SSID: %s", ssid);
                LOG("Password: %s", auth_param.au8PMK);

                LOG("Connecting to SSID: %s", ssid);
                res = m2m_wifi_connect(ssid, strlen(ssid), M2M_WIFI_SEC_WPA_PSK, &auth_param, M2M_WIFI_CH_ALL);
                if(res == M2M_SUCCESS)
                    LOG("Connection request sent");

                break;
            case NW_DISCONNECT:
                LOG("Disconnect command received");
                // Call the disconnect function here
                m2m_wifi_disconnect();
                LOG("Disconnect request sent");

                break;
            case NW_GET_MAC:
                LOG("Get MAC command received");
                get_mac();
                break;
            case NW_SEND_RAW:
                LOG("Send raw command received");
                // Call the send raw function here
                
                break;
            case NW_RECEIVE_RAW:
                LOG("Receive raw command received");
                // Call the receive raw function here
                break;
            case NW_RECEIVE_IRQ:
                LOG("Receive IRQ command received");
                m2m_wifi_handle_events(NULL);
                break;
            default:
                LOG("Unknown command received");
                break;
            
        } 
    }
}
