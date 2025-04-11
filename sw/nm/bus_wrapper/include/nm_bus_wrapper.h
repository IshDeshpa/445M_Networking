#ifndef _NM_BUS_WRAPPER_H_
#define _NM_BUS_WRAPPER_H_

#include <stdint.h>



typedef struct
{
	uint8_t	*pu8InBuf;		/*!< pointer to input buffer. 
							Can be set to null and in this case zeros should be sent at MOSI */
	uint8_t	*pu8OutBuf;		/*!< pointer to output buffer. 
							Can be set to null and in this case data from MISO can be ignored  */
	uint16_t	u16Sz;			/*!< Transfere size */	
} tstrNmSpiRw;

/*
 * nm_bus_init
 * Initialize the bus wrapper
 * Returns: 0 in case of success and M2M_ERR_BUS_FAIL in case of failure
 */ 
int8_t nm_bus_init();

/*
 * nm_bus_ioctl
 * Send/receive from the bus
 * Parameters:
 *   u8Cmd - IOCTL command for the operation
 *   pvParameter - Arbitrary parameter depending on IOCTL
 * Returns: 0 in case of success and M2M_ERR_BUS_FAIL in case of failure
 * Note: For SPI only, it's important to be able to send/receive at the same time
 */ 
int8_t nm_bus_ioctl(uint8_t u8Cmd, void* pvParameter);

/*
 * nm_bus_deinit
 * De-initialize the bus wrapper
 * Returns: 0 in case of success and M2M_ERR_BUS_FAIL in case of failure
 */ 
int8_t nm_bus_deinit(void);

/*
 * nm_bus_reinit
 * Re-initialize the bus wrapper
 * Parameters:
 *   config - Re-init configuration data
 * Returns: 0 in case of success and M2M_ERR_BUS_FAIL in case of failure
 */
int8_t nm_bus_reinit(void *config);

#endif