#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>

int putchar(int c);
int puts(const char *str);
int printf(const char *format, ...);
int vprintf(const char *format, va_list args);

#endif
