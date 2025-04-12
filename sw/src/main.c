/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "main.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/PLL.h"
#include "../inc/Timer0A.h"
#include "../inc/Timer1A.h"
#include "../inc/LaunchPad.h"
#include "../lib/std/stdio_lite/stdio_lite.h"
#include "../inc/UART.h"
#include "../inc/SysTick.h"
#include "../nm/bsp/nm_bsp.h"
#include "../nm/bus_wrapper/nm_bus_wrapper.h"
#include "../lib/std/stdio_lite/stdio_lite.h"
#include <stdint.h>

/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */

/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */

/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */
void StartupDelay(void);

/* ================================================== */
/*                    MAIN FUNCTION                   */
/* ================================================== */
static uint8_t inbuf[64]; // into the chip (out of TM4c)
static uint8_t outbuf[64]; // out of the chip (into TM4c)

int test_spi(void){
  tstrNmSpiRw spi_rw = {
    .pu8InBuf = inbuf,
    .pu8OutBuf = outbuf,
    .u16Sz = 16
  };

  for(int i=0;i<16;i++) inbuf[i] = i*2;

  nm_bus_ioctl(0, &spi_rw);

  while(1);
}

int test_spi_dma(void){
  tstrNmSpiRw spi_rw = {
    .pu8InBuf = inbuf,
    .pu8OutBuf = outbuf,
    .u16Sz = 64
  };

  for(int i=0;i<64;i++) inbuf[i] = i*2;

  nm_bus_ioctl(0, &spi_rw);

  while(1);
}

void TestIRQPin(void){
    printf("IRQ function reached\n\r");
}

void HeartBeat(void){
    GPIO_PORTF_DATA_R ^= 0x02;
    printf("thump\n\r");

}

int main(){
    DisableInterrupts();
    StartupDelay();
    PLL_Init(Bus80MHz);
    LaunchPad_Init();
    UART_Init();
    SysTick_Init();

    Timer0A_Init(HeartBeat, 80000, 7);
    //Timer1A_Init(void (*task)(void), uint32_t period, uint32_t priority)
    
    nm_bsp_init();
    nm_bsp_register_isr(TestIRQPin);

    nm_bus_init();

    //test_spi_dma();
    EnableInterrupts();

    while(1){

    }
}

void _putchar(char character){
  // send char to console etc.
  UART_OutChar(character);
}

void StartupDelay(void){
    for(uint16 i = 0; i < UINT16_MAX; i++){}
}
