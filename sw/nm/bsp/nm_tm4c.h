#ifndef _NM_BSP_TM4C_H_
#define _NM_BSP_TM4C_H_

#include "../conf_wilc.h"
// Use level-triggered IRQ (WILC IRQ is active-low and held until handled)
#define NM_LEVEL_INTERRUPT      (1)

// Enable WILC debug output
#define NM_DEBUG                (1)

// Route WILC driver logs to your printf implementation
#define NM_BSP_PRINTF(...)      printf(__VA_ARGS__)

#endif /* _NM_BSP_TM4C_H_ */
