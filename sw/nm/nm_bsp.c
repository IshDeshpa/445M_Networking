/**
 *
 * \file
 *
 * \brief This module contains SAMV71 BSP APIs implementation.
 *
 * Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "nm_bsp.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/SysTick.h"

/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */
/* Initialize reset, chip enable and wake pin */
static void init_chip_pins(void) {
    SYSCTL_RCGCGPIO_R |= 0x04;       // activate port C
    
    while((SYSCTL_RCGCGPIO_R&0x04)==0){};

    GPIO_PORTC_DIR_R |= 0xF0;        // make PC7-4 output
    GPIO_PORTC_AFSEL_R &= ~0xF0;     // disable alt funct on PC7-4
    GPIO_PORTC_DEN_R |= 0xF0;        // enable digital I/O on PC7-4
    GPIO_PORTC_PCTL_R &= ~0xFFFF0000;
    GPIO_PORTC_AMSEL_R &= ~0xF0;    // disable analog functionality on PC
    
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