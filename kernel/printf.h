#ifndef KERNEL_PRINTF_H
#define KERNEL_PRINTF_H

#include "kernel/stdarg.h"

int sprintf(char *str, const char *format, ...);
int vsprintf(char *str, const char *format, va_list ap);

#endif /* KERNEL_PRINTF_H */
