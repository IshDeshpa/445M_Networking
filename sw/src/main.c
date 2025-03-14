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
int buttunpressed;

/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */
void startupDelay(void);
void HeartBeat(void);
void PortD_Init(void);
/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */
#define PD0  (*((volatile uint32_t *)0x40007004))
#define PD1  (*((volatile uint32_t *)0x40007008))
#define PD2  (*((volatile uint32_t *)0x40007010))
#define PD3  (*((volatile uint32_t *)0x40007020))

int chipEnable, reset;
/* ================================================== */
/*                    MAIN FUNCTION                   */
/* ================================================== */
int main(void){
    startupDelay(); 
    DisableInterrupts();
    PLL_Init(Bus80MHz);
    LaunchPad_Init();
    UART_Init();
    PortD_Init();
    Timer0A_Init(HeartBeat, 80000000, 7);
    EnableInterrupts();

    buttunpressed = 0;
    while (1) {
        //if((GPIO_PORTF_DATA_R & 0x10) = 0x10){
        //    buttunpressed = 1;
        //}
    }
}

void HeartBeat(void){
    GPIO_PORTF_DATA_R ^= 0x02;
    static int count = 0;
    count++;
    if(count == 2){
        printf_lite("Enable High\n\r");
        PD0 |= 0x01;//enalbe
    }else if(count == 4){
        printf_lite("Reset High\n\r");
        PD1 |= 0x02;//reset
    }
}

void startupDelay(void){
  for(uint16_t i = 0; i < UINT16_MAX; i++){}
}

void PortD_Init(void){ 
    SYSCTL_RCGCGPIO_R |= 0x08;       // activate port D
    //
    while((SYSCTL_RCGCGPIO_R&0x08)==0){};      

    GPIO_PORTD_DIR_R |= 0x0F;        // make PD3-0 output heartbeats
    GPIO_PORTD_AFSEL_R &= ~0x1F;     // disable alt funct on PD3-0
    GPIO_PORTD_DEN_R |= 0x1F;        // enable digital I/O on PD3-0
    GPIO_PORTD_PCTL_R = ~0x000FFFFF;
    GPIO_PORTD_AMSEL_R &= ~0x1F;;    // disable analog functionality on PD

    GPIO_PORTD_DIR_R &= ~(0x10); //make pd5 a input
}

void _putchar(char character){
  // send char to console etc.
  UART_OutChar(character);
}
