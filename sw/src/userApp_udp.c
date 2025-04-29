/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "OS.h"
#include "Networking_Globs.h"
#include <stdint.h>
#include "UDP.h"
#include "string_lite.h"
#include "userApp_udp.h"
/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */
#define USERAPP_DATA_FIFO_SIZE (3)
#define USERAPP_PAYLOAD_SIZE (128)
#define TEST_TX_IP (0x12345678)
#define TEST_TX_PORT

OS_FIFO_t userAppFifo;
sema4_t userAppFifo_sema4s[3];
uint8_t userAppFifo_buffer[USERAPP_DATA_FIFO_SIZE];

uint8_t payloadRXBuf[USERAPP_PAYLOAD_SIZE];
uint8_t userAppTxBuf[MTU];

const uint8_t testpayload[] = {
    0x3A, 0xB7, 0x1F, 0x92, 0x6C, 0xD0, 0x8E, 0x15,
    0xF4, 0x2D, 0x7A, 0xE1, 0x9B, 0x43, 0xC6, 0x5E,
    0x01, 0xAD, 0x7F, 0x68, 0x94, 0xD7, 0x3C, 0x2B,
    0xEF, 0x88, 0x36, 0x10, 0x77, 0x5A, 0xC9, 0x42,
    0xE5
};
/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */
void userPrint_RXData(uint8_t* payload, uint16_t size);
/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */
void Task_UserAPP_UdpRX(void){
    while(OS_Fifo_Get(payloadRXBuf, &userAppFifo)){
        userPrint_RXData(payloadRXBuf, sizeof(payloadRXBuf));
    }
}

void Task_TestUDPTx(void){
    while(1){
        LOG("Sending User App Data");
        OS_Sleep(5000);
        memcpy(userAppTxBuf, testpayload, sizeof(testpayload));
        udp_tx(sizeof(testpayload), userAppTxBuf, TEST_TX_IP, USERAPP_UDP_PORT, TEST_TX_PORT);
    }
}

void userAppApp_UdpExInit(void){
    OS_Fifo_Init(USERAPP_DATA_FIFO_SIZE, &userAppFifo, userAppFifo_buffer, USERAPP_PAYLOAD_SIZE, userAppFifo_sema4s);
    OS_AddThread(Task_TestUDPTx, STACKSIZE, 5);
    OS_AddThread(Task_UserAPP_UdpRX, STACKSIZE, 5);
}


void userPrint_RXData(uint8_t* payload, uint16_t size){
    printf("========== Received Payload (%u bytes) ==========\n", size);
    for (uint16_t i = 0; i < size; i++) {
        if (i % 8 == 0) {
            printf("%04X: ", i);  // offset label
        }

        printf("%02X ", payload[i]);

        if ((i + 1) % 8 == 0 || i == size - 1) {
            printf("\n");
        }
    }
    printf("=================================================\n");
}
