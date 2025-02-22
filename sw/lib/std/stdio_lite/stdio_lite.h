#ifndef STRING_LITE_H
#define STRING_LITE_H

/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include <stdarg.h>
#include <stdint.h>
/* ================================================== */
/*            GLOBAL VARIABLE DECLARATIONS            */
/* ================================================== */

/* ================================================== */
/*                 FUNCTION PROTOTYPES                */
/* ================================================== */
void _putchar(char character);

void printf_lite(const char* format, ...);

void sprintf_lite(char* buffer, const char* format, ...);

void snprintf_lite(char* buffer, int buffer_size, const char* format, ...);

void itoa_lite(int32_t num, char* buffer);

#endif 

