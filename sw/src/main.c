/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "main.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/PLL.h"
#include "../inc/Timer0A.h"
#include "../inc/Timer1A.h"
#include "../inc/LaunchPad.h"
#include "../inc/UART.h"
#include "../inc/SysTick.h"

#include "../nm/bsp/nm_bsp.h"
#include "../nm/bus_wrapper/nm_bus_wrapper.h"
#include "../nm/driver/include/m2m_wifi.h"

#include "Networking.h"
#include "../lib/std/stdio_lite/stdio_lite.h"
//#include "../lib/std/printf/printf.h"
#include <stdint.h>

/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */
#define PD0  (*((volatile uint32_t *)0x40007004))
#define PD1  (*((volatile uint32_t *)0x40007008))
#define PD2  (*((volatile uint32_t *)0x40007010))
#define PD3  (*((volatile uint32_t *)0x40007020))
/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */
void PortD_Init(void);

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
    printf("statring printf test\n\r");
  nm_bus_ioctl(0, &spi_rw);
    printf("passed spi test\n\r");
  while(1);
}

int test_spi_dma(void){
  tstrNmSpiRw spi_rw = {
    .pu8InBuf = inbuf,
    .pu8OutBuf = outbuf,
    .u16Sz = 64
  };

  for(int i=0;i<64;i++) inbuf[i] = i*2;
    while(1){
        nm_bus_ioctl(0, &spi_rw);
        nm_bsp_sleep(80000000);
    }
  while(1){}
}

void TestIRQPin(void){
    printf("IRQ function reached\n\r");
}

void HeartBeat(void){
    PD0 ^= 0x01;
    //printf("thump\n\r");

}

int main(){
    DisableInterrupts();
    StartupDelay();
    PLL_Init(Bus80MHz);
    //LaunchPad_Init();
    UART_Init();
    SysTick_Init();
    PortD_Init();
    Timer0A_Init(HeartBeat, 80000000, 7);

    //Wifi_Init(); 
    nm_bsp_init();
    nm_bus_init(NULL);
    //get_mac_test();
    //Timer1A_Init(void (*task)(void), uint32_t period, uint32_t priority)
    //nm_bsp_init();
    //nm_bsp_register_isr(TestIRQPin);

    //nm_bus_init();
    test_spi();
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


void PortD_Init(void){ 
  SYSCTL_RCGCGPIO_R |= 0x08;       // activate port D
  while((SYSCTL_RCGCGPIO_R&0x08)==0){};      
  GPIO_PORTD_DIR_R |= 0xFF;        // make PD3-0 output heartbeats
  GPIO_PORTD_AFSEL_R &= ~0xFF;     // disable alt funct on PD3-0
  GPIO_PORTD_DEN_R |= 0xFF;        // enable digital I/O on PD3-0
  GPIO_PORTD_PCTL_R = ~0xFFFFFFFF;
  GPIO_PORTD_AMSEL_R &= ~0xFF;;    // disable analog functionality on PD
}

