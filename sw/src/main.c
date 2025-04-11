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

int main(){
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  LaunchPad_Init();
  UART_Init();
  SysTick_Init();
  
  nm_bsp_init();
  nm_bus_init();

  test_spi_dma();
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

