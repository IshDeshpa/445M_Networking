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
/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */


/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */
void startupDelay(void);
void HeartBeat(void);
/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */


/* ================================================== */
/*                    MAIN FUNCTION                   */
/* ================================================== */
int main(void){
    startupDelay(); 
    DisableInterrupts();
    PLL_Init(Bus80MHz);
    LaunchPad_Init();
    UART_Init();
    Timer0A_Init(HeartBeat, 80000000, 7);
    EnableInterrupts();
  while (1) {

  }
}

void HeartBeat(void){
    static int count = 0;
    GPIO_PORTF_DATA_R ^= 0x02;//toggle pf1, red led
    printf_lite("thummp %d\n\r", count);
    count++;
}
void startupDelay(void){
  for(uint16_t i = 0; i < UINT16_MAX; i++){}
}

void _putchar(char character){
  // send char to console etc.
  UART_OutChar(character);
}
