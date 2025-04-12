/** \defgroup nm_bsp BSP
 *
 * \file
 *
 * \brief This module contains NMC1500 BSP APIs declarations.
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
#ifndef _NM_BSP_H_
#define _NM_BSP_H_

#include <stdint.h>

#include "../../inc/tm4c123gh6pm.h"
#include "nm_tm4c.h"
#define NMI_API
#ifdef __MCF964548__
#define CONST code
#else
#define CONST const
#endif
/*!< 
*     Used for code portability.
*/

typedef void (*tpfNmBspIsr)(void);



#ifndef NULL
#define NULL ((void*)0)
#endif

#define BSP_MIN(x,y) ((x)>(y)?(y):(x))

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned long	uint32;
typedef signed char		sint8;
typedef signed short	sint16;
typedef signed long		sint32;

#define __M2M_DMA_BUF_ATT__

#ifdef _NM_BSP_BIG_END
#define NM_BSP_B_L_32(x) \
((((x) & 0x000000FF) << 24) + \
(((x) & 0x0000FF00) << 8)  + \
(((x) & 0x00FF0000) >> 8)   + \
(((x) & 0xFF000000) >> 24))
#define NM_BSP_B_L_16(x) \
((((x) & 0x00FF) << 8) + \
(((x)  & 0xFF00) >> 8))
#else
#define NM_BSP_B_L_32(x)  (x)
#define NM_BSP_B_L_16(x)  (x)
#endif



#define WILC_DFL_CTRL_PORT (GPIO_PORTE_DATA_R)

//make pe0 the reset pin
#define WILC_RESET_PORT (WILC_DFL_CTRL_PORT)
#define WILC_RESET_PIN (1<<0)

// pe1 the chipen 
#define WILC_CHIPEN_PORT (WILC_DFL_CTRL_PORT)
#define WILC_CHIPEN_PIN (1<<1)

//pe2 the wake pin
#define WILC_WAKE_PORT (WILC_DFL_CTRL_PORT)
#define WILC_WAKE_PIN (1<<2)

// Set all pins directly by manipulating the register
#define WILC_RESET_HI     (WILC_DFL_CTRL_PORT |= WILC_RESET_PIN)
#define WILC_RESET_LO     (WILC_DFL_CTRL_PORT &= ~WILC_RESET_PIN)

#define WILC_CHIPEN_HI    (WILC_DFL_CTRL_PORT |= WILC_CHIPEN_PIN)
#define WILC_CHIPEN_LO    (WILC_DFL_CTRL_PORT &= ~WILC_CHIPEN_PIN)

#define WILC_WAKE_HI      (WILC_DFL_CTRL_PORT |= WILC_WAKE_PIN)
#define WILC_WAKE_LO      (WILC_DFL_CTRL_PORT &= ~WILC_WAKE_PIN)
/* 
 * Initializes the Board Support Package (BSP), including reset and chip enable pins for WILC, delays, 
 * registering ISR, and enabling/disabling IRQ for WILC. This function must be called at the start of 
 * the application to enable communication between WILC and the host driver.
 * Returns M2M_SUCCESS on success, or a negative value on failure.
 */
sint8  nm_bsp_init(void);

/* 
 * Deinitializes the BSP. This function should be called after nm_bsp_init. Missing this call may lead 
 * to undefined behavior during a soft reset. Returns M2M_SUCCESS on success, or a negative value on failure.
 */
sint8  nm_bsp_deinit(void);

/* 
 * Resets the NMC1500 SoC by toggling the CHIP_EN and RESET_N signals. This function is host-dependent 
 * and is called by the HIF layer. Ensure nm_bsp_init is called before using this function.
 */
void   nm_bsp_reset(void);

/* 
 * Puts the system to sleep for the specified time in milliseconds. The maximum value is 4294967295 ms 
 * (approximately 4294967.295 seconds). This function is host-dependent and is used by the HIF layer.
 */
void nm_bsp_sleep(uint32_t u32TimeMsec);

void   nm_bsp_register_isr(tpfNmBspIsr pfIsr);

void   nm_bsp_interrupt_ctrl(uint8 u8Enable);

void*  nm_bsp_malloc(uint32 u32Size);

void   nm_bsp_free(void* pvMemBuffer);
#endif
