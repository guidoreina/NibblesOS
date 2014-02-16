#include "kernel/string.h"

size_t strlen(const char *s)
{
	const char *p = s;

	while (*p) {
		p++;
	}

	return p - s;
}

size_t strnlen(const char *s, size_t maxlen)
{
	const char *p = s;

	while ((*p) && (maxlen > 0)) {
		p++;
		maxlen--;
	}

	return p - s;
}

void *memset(void *s, int c, size_t n)
{
	uint8_t *p = (uint8_t *) s;

	while (n-- > 0) {
		*p++ = c;
	}

	return s;
}

void *memcpy(void *dest, const void *src, size_t n)
{
	uint8_t *d = (uint8_t *) dest;
	const uint8_t *s = (const uint8_t *) src;

	while (n-- > 0) {
		*d++ = *s++;
	}

	return dest;
}

void *memmove(void *dest, const void *src, size_t n)
{
	if (dest < src) {
		uint8_t *d = (uint8_t *) dest;
		const uint8_t *s = (const uint8_t *) src;

		while (n-- > 0) {
			*d++ = *s++;
		}
	} else if (dest > src) {
		uint8_t *d = (uint8_t *) dest + n;
		const uint8_t *s = (const uint8_t *) src + n;

		while (n-- > 0) {
			*--d = *--s;
		}
	}

	return dest;
}
