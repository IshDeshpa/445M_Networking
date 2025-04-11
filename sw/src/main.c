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

    while(1);
}

void _putchar(char character){
  // send char to console etc.
  UART_OutChar(character);
}


//sets up GPIO rising edge interrupts on porte 1-3
void PortE_Init(void){
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R4) == 0);

    GPIO_PORTE_DIR_R &= ~0x0E;
    GPIO_PORTE_AFSEL_R &= ~0x0E;
    GPIO_PORTE_DEN_R |= 0x0E;
    GPIO_PORTE_AMSEL_R &= ~0x0E;
    GPIO_PORTE_PCTL_R &= ~0x0000FFF0;
    
    GPIO_PORTE_IS_R &= ~0x0E;//makes these pins edge sensitive
    GPIO_PORTE_IBE_R &= ~0x0E;//turns off both edge interrutps
    GPIO_PORTE_IEV_R &= ~0x0E; //risnig edge   
    GPIO_PORTE_ICR_R |= 0x0E;//clears flags
    GPIO_PORTE_IM_R |= 0x0E;//arms the interrupts

    // Set interrupt 4 (Port E) to priority 4
    // Set Port E (Interrupt 4) priority to 4
    NVIC_PRI1_R = (NVIC_PRI1_R & ~0x000000E0) | (1 << 5);


    NVIC_EN0_R |= 0x10;//enable porte intterupt in nvic
}

void GPIOPortE_Handler(void){
    uint32_t pinOfInterest = GPIO_PORTE_RIS_R & 0x0E;//pull out pe1-3
    GPIO_PORTE_ICR_R |= pinOfInterest;//awcknoleding interupt
}

void DisableHandlerInts(void){
    GPIO_PORTE_IM_R &= ~0x02;
}

void EnableHandlerInts(void){
    GPIO_PORTE_IM_R |= 0x02;
}

