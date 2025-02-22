#ifndef STRING_LITE_H
#define STRING_LITE_H

/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include <stdint.h>
/* ================================================== */
/*            GLOBAL VARIABLE DECLARATIONS            */
/* ================================================== */

/* ================================================== */
/*                 FUNCTION PROTOTYPES                */
/* ================================================== */
int strlen_lite(const char* str);

void strcpy_lite(char* dest, const char* src);

void strncpy_lite(char* dest, const char* src, int n);

void strcat_lite(char* dest, const char* src);

void strncat_lite(char* dest, const char* src, int n);

int strcmp_lite(const char* str1, const char* str2);

int strmcmp_lite(const char* str1, const char* str2, int n);

char *strchr_lite(const char *str, int c);

char *strtok_lite(char *str, const char *delim);

#endif 

