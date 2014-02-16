#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H

#include "kernel/types.h"

size_t strlen(const char *s);
size_t strnlen(const char *s, size_t maxlen);
void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);

#endif /* KERNEL_STRING_H */
