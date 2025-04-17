/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "CLI_Commands.h"
#include "../../lib/std/stdio_lite/stdio_lite.h"
/* ================================================== */
/*            GLOBAL VARIABLE DECLARATIONS            */
/* ================================================== */
/* ================================================== */
/*                 FUNCTION PROTOTYPES                */
/* ================================================== */

/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */
CMDS_RESULT CLI_rm(char* fileName){
    CLI_Wait();
    if(eFile_Delete(fileName)){ 
        printf("Error Deleting File, prob not found :(\n\r");
        CLI_Signal();
        return RM_FAIL;
    }
    CLI_Signal();
    return CMDS_SUCCESS;
}

