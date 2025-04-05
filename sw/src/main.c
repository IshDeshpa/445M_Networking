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
#include "../nm/nm_bsp.h"
#include "../nm/nm_bus_wrapper.h"

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

int chipEnable, reset;
/* ================================================== */
/*                    MAIN FUNCTION                   */
/* ================================================== */
int main(void){
    for(uint16_t i = 0; i < UINT16_MAX; i++){}
    DisableInterrupts();
    PLL_Init(Bus80MHz);
    LaunchPad_Init();
    UART_Init();
    SysTick_Init();
    
    nm_bsp_init();
    nm_bus_init();

    while(1);
}

void _putchar(char character){
  // send char to console etc.
  UART_OutChar(character);
}
