/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "main.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/PLL.h"
#include "../inc/Timer0A.h"
#include "../inc/LaunchPad.h"
#include "../lib/std/stdio_lite/stdio_lite.h"
#include "../inc/UART.h"
#include "../inc/SysTick.h"
#include "../nm/bsp/include/nm_bsp.h"
#include "../nm/bus_wrapper/include/nm_bus_wrapper.h"

#include <stdint.h>
#include <string.h>

/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */

/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */

/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */

int chipEnable, reset;
/* ================================================== */
/*                    MAIN FUNCTION                   */
/* ================================================== */
static uint8_t inbuf[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
static uint8_t outbuf[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

int main(void){
    for(uint16_t i = 0; i < UINT16_MAX; i++){}
    DisableInterrupts();
    PLL_Init(Bus80MHz);
    LaunchPad_Init();
    UART_Init();
    SysTick_Init();
    
    nm_bsp_init();
    nm_bus_init();

    tstrNmSpiRw spi_rw = {
      .pu8InBuf = inbuf,
      .pu8OutBuf = outbuf,
      .u16Sz = 16
    };

    // memset(outbuf, 0xFF, sizeof(outbuf));

    nm_bus_ioctl(0, &spi_rw);

    while(1);
}

void _putchar(char character){
  // send char to console etc.
  UART_OutChar(character);
}
