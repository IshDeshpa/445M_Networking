/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "OS.h"
#include <stdint.h>

/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */
#define USERAPP_DATA_FIFO_SIZE (3)
OS_FIFO_t userFifo;
sema4_t userFifo_sema4s[3];
uint8_t userFifo_buffer[USERAPP_DATA_FIFO_SIZE];
/* ================================================== */
/*            FUNCTION PROTOTYPES (DECLARATIONS)      */
/* ================================================== */


/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */
void UserApp_UdpExInit(void){

}

void Task_
