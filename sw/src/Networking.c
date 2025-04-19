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

sema4_t num_channels_sem4;
uint8_t num_channels;

void wifi_callback(uint8 u8MsgType, void *pvMsg) {
    LOG("Wi-Fi callback triggered! Message type: %d", u8MsgType);
    
    tenuM2mStaCmd val = u8MsgType;

    switch (u8MsgType) {
        case M2M_WIFI_REQ_CONNECT:
            LOG("Wi-Fi request to connect received.");
            break;
        case M2M_WIFI_REQ_GET_CONN_INFO:
            LOG("Wi-Fi request to get connection info received.");
            break;
        case M2M_WIFI_RESP_CONN_INFO:
            LOG("Wi-Fi response with connection info received.");
            break;
        case M2M_WIFI_REQ_DISCONNECT:
            LOG("Wi-Fi request to disconnect received.");
            break;
        case M2M_WIFI_RESP_CON_STATE_CHANGED:
            LOG("Wi-Fi connection state changed.");
            break;
        case M2M_WIFI_REQ_SLEEP:
            LOG("Wi-Fi request to sleep received.");
            break;
        case M2M_WIFI_REQ_SCAN:
            LOG("Wi-Fi scan request received.");
            break;
        case M2M_WIFI_RESP_SCAN_DONE:
            LOG("Wi-Fi scan done response received.");
            tstrM2mScanDone *pstrState = (tstrM2mScanDone *)pvMsg;


            break;
        case M2M_WIFI_REQ_SCAN_RESULT:
            LOG("Wi-Fi scan result request received.");
            break;
        case M2M_WIFI_RESP_SCAN_RESULT:
            LOG("Wi-Fi scan result response received.");
            break;
        case M2M_WIFI_REQ_START_WPS:
            LOG("Wi-Fi request to start WPS received.");
            break;
        case M2M_WIFI_REQ_DISABLE_WPS:
            LOG("Wi-Fi request to disable WPS received.");
            break;
        case M2M_WIFI_RES_WPS:
            LOG("Wi-Fi WPS response received.");
            break;
        case M2M_WIFI_RESP_IP_CONFIGURED:
            LOG("Wi-Fi IP configured response received.");
            break;
        case M2M_WIFI_RESP_IP_CONFLICT:
            LOG("Wi-Fi IP conflict response received.");
            break;
        case M2M_WIFI_REQ_ENABLE_MONITORING:
            LOG("Wi-Fi request to enable monitoring received.");
            break;
        case M2M_WIFI_REQ_DISABLE_MONITORING:
            LOG("Wi-Fi request to disable monitoring received.");
            break;
        case M2M_WIFI_RESP_WIFI_RX_PACKET:
            LOG("Wi-Fi RX packet response received.");
            break;
        case M2M_WIFI_REQ_SEND_WIFI_PACKET:
            LOG("Wi-Fi request to send packet received.");
            break;
        case M2M_WIFI_REQ_LSN_INT:
            LOG("Wi-Fi listen interval request received.");
            break;
        case M2M_WIFI_REQ_SEND_ETHERNET_PACKET:
            LOG("Wi-Fi request to send Ethernet packet received.");
            break;
        case M2M_WIFI_RESP_ETHERNET_RX_PACKET:
            LOG("Wi-Fi Ethernet RX packet response received.");
            break;
        case M2M_WIFI_REQ_SET_SCAN_OPTION:
            LOG("Wi-Fi request to set scan option received.");
            break;
        case M2M_WIFI_REQ_SET_SCAN_REGION:
            LOG("Wi-Fi request to set scan region received.");
            break;
        case M2M_WIFI_REQ_SET_SCAN_LIST:
            LOG("Wi-Fi request to set scan list received.");
            break;
        case M2M_WIFI_REQ_SET_MAC_MCAST:
            LOG("Wi-Fi request to set MAC multicast received.");
            break;
        case M2M_WIFI_REQ_SET_P2P_IFC_ID:
            LOG("Wi-Fi request to set P2P interface ID received.");
            break;
        case M2M_WIFI_RESP_PACKET_SENT:
            LOG("Wi-Fi packet sent response received.");
            break;
        case M2M_WIFI_REQ_CERT_ADD_CHUNK:
            LOG("Wi-Fi request to add certificate chunk received.");
            break;
        case M2M_WIFI_REQ_CERT_DOWNLOAD_DONE:
            LOG("Wi-Fi certificate download done request received.");
            break;
        case M2M_WIFI_REQ_CHG_MONITORING_CHNL:
            LOG("Wi-Fi request to change monitoring channel received.");
            break;
        default:
            LOG("Unknown Wi-Fi message type received.");
            break;
    }
}

void print_mac(uint8_t *mac) {
    printf("%02X:%02X:%02X:%02X:%02X:%02X\n\r",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

uint8_t irq_rcv_buf[2048];

errNetworking_t Wifi_Init(void){

    // nm_bsp_init();

    LOG("Starting Up Wifi Init\n\r");

    wifi_init_param.pfAppWifiCb = &wifi_callback;
    wifi_init_param.pfAppMonCb = NULL;
    wifi_init_param.strEthInitParam.au8ethRcvBuf = irq_rcv_buf;
    wifi_init_param.strEthInitParam.u16ethRcvBufSize = 2048;
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

#define NETWORK_COMMAND_FIFO_SIZE 32
OS_FIFO_t network_command_fifo;
sema4_t network_command_sema4[3];
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

void Network_Request_Scan_result(void){
    network_command_t cmd;
    cmd.command = NW_SCAN;
    memset(cmd.data, 0, sizeof(cmd.data));

    OS_Fifo_Put((uint8_t*)&cmd, &network_command_fifo);
}

void Network_Get_Mac(void){
    network_command_t cmd;
    cmd.command = NW_GET_MAC;
    memset(cmd.data, 0, sizeof(cmd.data));
    OS_Fifo_Put((uint8_t*)&cmd, &network_command_fifo);
}

void Task_NetworkThread(void){
    OS_Fifo_Init(NETWORK_COMMAND_FIFO_SIZE, 
                &network_command_fifo, 
                (uint8_t*)network_command_fifo_buffer,
                sizeof(network_command_t),
                network_command_sema4);
    OS_InitSemaphore(&num_channels_sem4, 0);

    nm_bsp_init();
    LOG("NM BSP init finished\n\r");
    Wifi_Init();
    LOG("Wifi Init finished\n\r");
    
    while(1){
        // Handle incoming commands
        network_command_t cmd;
        OS_Fifo_Get((uint8_t*)&cmd, &network_command_fifo);
        
        // LOG("Command received: %d", cmd.command);

        // LOG("Fifo dump:");
        // OS_Fifo_Print(&network_command_fifo);

        sint8 res;
        switch(cmd.command){
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
                
                strncpy(ssid, cmd.data, 32);
                strncpy(auth_param.au8PMK, cmd.data + 32, 40);
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
