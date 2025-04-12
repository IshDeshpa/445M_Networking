/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "nm_bsp.h"
#include "nm_tm4c.h"
#include "../../inc/tm4c123gh6pm.h"
#include "../../inc/SysTick.h"

/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */
#define IRQ_PIN_BIT 0x10
void(*gpio_irq_func)(void) = NULL;

/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */

/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */
/* Initialize reset, chip enable and wake pin */
static void init_chip_pins(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R4) == 0);

    GPIO_PORTE_DIR_R |=  0x2F;       // Set PE0–3 and PE5 as outputs
    GPIO_PORTE_DIR_R &= ~0x10;       // Clear PE4 for input

    GPIO_PORTE_DEN_R |= 0x3F;

    GPIO_PORTE_AFSEL_R &= ~0x3F;
    GPIO_PORTE_AMSEL_R &= ~0x3F;
    GPIO_PORTE_PCTL_R &= ~0x00FFFFFF;  // Clear PCTL for PE0–PE5

    GPIO_PORTE_IS_R &= ~0x10;        // PE4 edge-sensitive
    GPIO_PORTE_IBE_R &= ~0x10;       // Not both edges
    GPIO_PORTE_IEV_R &= ~0x10;       // Falling edge
    GPIO_PORTE_ICR_R |=  0x10;       // Clear any prior interrupt flag
    GPIO_PORTE_IM_R |=  0x10;        // Arm interrupt on PE4

    NVIC_PRI1_R = (NVIC_PRI1_R & ~0x000000E0) | (CONF_WILC_IRQ_PRI << 5);  // Priority 2

    NVIC_EN0_R |= (1 << 4);
    WILC_RESET_LO;
}

/* Initialize BSP */
int8_t nm_bsp_init(void) {
    init_chip_pins();
    nm_bsp_reset();
    return 0;
}

int8_t nm_bsp_deinit(void) {
    WILC_CHIPEN_LO;
    WILC_RESET_LO;
    return 0;
}

/* Reset NMC1500 SoC by setting CHIP_EN and RESET_N signals low,
   CHIP_EN high then RESET_N high */
void nm_bsp_reset(void) {
    WILC_CHIPEN_LO;
	WILC_RESET_LO;
	nm_bsp_sleep(100);
	WILC_CHIPEN_HI;
	nm_bsp_sleep(100);
	WILC_RESET_HI;
	nm_bsp_sleep(100);
}

/* Sleep in units of mSec */
void nm_bsp_sleep(uint32_t u32TimeMsec) {
    // TODO: Replace with OS_Sleep
    SysTick_Wait(u32TimeMsec * 80000); // Assuming 80MHz clock
}


void GPIOPortE_Handler(void){
    GPIO_PORTE_ICR_R |= IRQ_PIN_BIT;//awcknoleding interupt
    gpio_irq_func();
}

void DisableHandlerInts(void){
    GPIO_PORTE_IM_R &= ~0x02;
}

void EnableHandlerInts(void){
    GPIO_PORTE_IM_R |= 0x02;
}

//think is just gonna be calloed in portC handler
void   nm_bsp_register_isr(tpfNmBspIsr pfIsr){
    gpio_irq_func = pfIsr;    
}

//maybe to turn off or clear flag ?
void   nm_bsp_interrupt_ctrl(uint8 u8Enable){
    if(u8Enable){
        EnableHandlerInts();
    }else{
        DisableHandlerInts();
    }
}

//we can just pull code from lab5 and route it
void*  nm_bsp_malloc(uint32 u32Size);

void   nm_bsp_free(void* pvMemBuffer);
