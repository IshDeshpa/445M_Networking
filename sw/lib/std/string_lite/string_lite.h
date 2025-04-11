#ifndef STRING_LITE_H
#define STRING_LITE_H

/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include <stddef.h>
#include <stdint.h>
/* ================================================== */
/*            GLOBAL VARIABLE DECLARATIONS            */
/* ================================================== */

#define strlen   strlen_lite
#define strcpy   strcpy_lite
#define strncpy  strncpy_lite
#define strcat   strcat_lite
#define strncat  strncat_lite
#define strcmp   strcmp_lite
#define strncmp  strncmp_lite
#define strchr   strchr_lite
#define strtok   strtok_lite
#define strstr   strstr_lite 
#define atol     atol_lite

/* ================================================== */
/*                 FUNCTION PROTOTYPES                */
/* ================================================== */

int strlen_lite(const char* str);

void strcpy_lite(char* dest, const char* src);

void strncpy_lite(char* dest, const char* src, int n);

void strcat_lite(char* dest, const char* src);

void strncat_lite(char* dest, const char* src, int n);

int strcmp_lite(const char* str1, const char* str2);

int strncmp_lite(const char* str1, const char* str2, int n);

char *strchr_lite(const char *str, int c);

char *strtok_lite(char *str, const char *delim);

void* memcpy(void* dest, const void* src, size_t n);

char* strstr_lite(const char* haystack, const char* needle);

long atol_lite(const char* str);
#endif 

