/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "Networking.h"
#include <stdint.h>
#include "../lib/std/stdio_lite/stdio_lite.h"

#include "../nm/bsp/nm_bsp.h"
#include "../nm/bus_wrapper/nm_bus_wrapper.h"
#include "../nm/driver/include/m2m_wifi.h"
/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */
#define LOG printf


tstrWifiInitParam wifi_init_param;
/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */


/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */

void wifi_callback(uint8 u8MsgType, void *pvMsg) {
    LOG("Wi-Fi callback triggered! Message type: %d\n\r", u8MsgType);
}

void print_mac(uint8 *mac) {
    printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void get_mac_test() {
    uint8 mac_ap[6];    // MAC for AP interface
    uint8 mac_sta[6];   // MAC for STA interface

    if (m2m_wifi_get_mac_address(mac_ap, mac_sta) == M2M_SUCCESS) {
        LOG("AP MAC:  ");
        print_mac(mac_ap);

        LOG("STA MAC: ");
        print_mac(mac_sta);
    } else {
        LOG("Failed to get MAC addresses\n");
    }
}

int Wifi_Init(void){

    nm_bsp_init();
    LOG("nm bsp init finised, gpio is on :)\n\r");

    LOG("Starting Up Wifi Init\n\r");

    wifi_init_param.pfAppWifiCb = wifi_callback;
    wifi_init_param.pfAppMonCb = NULL;
    wifi_init_param.strEthInitParam.au8ethRcvBuf = NULL;
    wifi_init_param.strEthInitParam.u16ethRcvBufSize = 0;
    wifi_init_param.strEthInitParam.pfAppEthCb = NULL;
    int8_t ret = m2m_wifi_init(&wifi_init_param);
    if(ret == M2M_SUCCESS){
        LOG("Wifi Init Sucess\n\r");
    }else{
       LOG("Failed with exit code: %d\n\r", ret);
        return WIFI_INIT_FAIL;
    }

    
}

