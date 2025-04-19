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
