#ifndef PRINT_H
#define PRINT_H

#include <stdlib.h>  // for itoa() and utoa()
#include <string.h>  // for strlen() and strcat()
#include <stdarg.h>  // for va_start(), va_end(), va_arg() and va_copy()

char* ulltoa(unsigned long long value, char* str, int base);

void format_to_str(char* out, const char* fmt, va_list args);

unsigned int format_to_str_len(const char* fmt, va_list args_copy);

int printf(const char* format, ...);
#endif
