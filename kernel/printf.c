#include "kernel/printf.h"
#include "kernel/string.h"
#include "macros/macros.h"

/* Based in Linux's source code: /arch/x86/boot/printf.c */

typedef enum {
	NO_FLAGS = 0,
	ZERO_PAD = 1,
	SIGNED = 2,
	LEFT = 4,
	SPACE = 8,
	PLUS = 16,
	POINTER = 32,
	CAPITAL = 64
} flags_t;

/*******************************************************************************
 *******************************************************************************
 **                                                                           **
 ** Local functions.                                                          **
 **                                                                           **
 *******************************************************************************
 *******************************************************************************/

static int atoi(const char *str, size_t *len)
{
	const char *p = str;
	int ret = 0;

	while (IS_DIGIT(*p)) {
		ret = (ret * 10) + (*p++ - '0');
	}

	*len = p - str;

	return ret;
}

static inline int process_flag(char c, flags_t *flags)
{
	switch (c) {
		case '0':
			*flags |= ZERO_PAD;
			return 1;
		case '-':
			*flags |= LEFT;
			return 1;
		case ' ':
			*flags |= SPACE;
			return 1;
		case '+':
			*flags |= PLUS;
			return 1;
		default:
			return 0;
	}
}

static size_t number(char *str, long n, int base, int field_width, int precision, int flags)
{
	static const char xdigits[] = "0123456789abcdef";
	static const char Xdigits[] = "0123456789ABCDEF";
	const char *digits = (flags & CAPITAL) ? Xdigits : xdigits;
	char pad = (flags & ZERO_PAD) ? '0' : ' ';
	char *dest;
	char num[64];
	int numlen;
	char sign;

	/* If the 0 and - flags both appear, the 0 flag is ignored. */
	if (flags & LEFT) {
		flags &= ~ZERO_PAD;
	}

	if (flags & SIGNED) {
		if (n < 0) {
			sign = '-';
			n = -n;
			field_width--;
		} else if (flags & PLUS) {
			sign = '+';
			field_width--;
		} else if (flags & SPACE) {
			sign = ' ';
			field_width--;
		} else {
			sign = 0;
		}
	} else {
		sign = 0;

		if (flags & POINTER) {
			field_width -= 2;
		}
	}

	numlen = 0;
	while (n >= base) {
		num[numlen++] = digits[n % base];
		n /= base;
	}

	num[numlen++] = digits[n];

	if (numlen > precision) {
		precision = numlen;
	}

	field_width -= precision;

	dest = str;

	if ((flags & (ZERO_PAD | LEFT)) == 0) {
		while (field_width-- > 0) {
			*dest++ = ' ';
		}
	}

	if (sign) {
		*dest++ = sign;
	} else {
		if (flags & POINTER) {
			*dest++ = '0';
			*dest++ = 'x';
		}
	}

	/* Right justification? */
	if ((flags & LEFT) == 0) {
		while (field_width-- > 0) {
			*dest++ = pad;
		}
	}

	while (numlen < precision--) {
		*dest++ = '0';
	}

	while (numlen > 0) {
		*dest++ = num[--numlen];
	}

	while (field_width-- > 0) {
		*dest++ = ' ';
	}

	return dest - str;
}



/*******************************************************************************
 *******************************************************************************
 **                                                                           **
 ** Global functions.                                                         **
 **                                                                           **
 *******************************************************************************
 *******************************************************************************/

int sprintf(char *str, const char *format, ...)
{
	va_list ap;
	int ret;

	va_start(ap, format);
	ret = vsprintf(str, format, ap);
	va_end(ap);

	return ret;
}

int vsprintf(char *str, const char *format, va_list ap)
{
	char *dest;
	const char *s;
	flags_t flags;
	int field_width;
	int precision;
	int len;
	int i;

	dest = str;

	while (*format) {
		if (*format != '%') {
			*dest++ = *format++;
			continue;
		}

		/* Skip character '%'. */
		format++;

		/* Process flags (if present). */
		flags = NO_FLAGS;
		while (process_flag(*format, &flags)) {
			format++;
		}

		/* Get field width (if present). */
		field_width = -1;
		if (IS_DIGIT(*format)) {
			field_width = atoi(format, (size_t *) &len);
			format += len;
		} else if (*format == '*') {
			format++;
			if ((field_width = va_arg(ap, int)) < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* Get precision (if present). */
		precision = -1;
		if (*format == '.') {
			format++;

			if (IS_DIGIT(*format)) {
				precision = atoi(format, (size_t *) &len);
				format += len;
			} else if (*format == '*') {
				format++;
				precision = va_arg(ap, int);
			}

			if (precision < 0) {
				precision = 0;
			}
		}

		/* Process conversion specifier. */
		switch (*format) {
			case 'c':
				/* Right justification? */
				if ((flags & LEFT) == 0) {
					while (--field_width > 0) {
						*dest++ = ' ';
					}

					*dest++ = va_arg(ap, int);
				} else {
					*dest++ = va_arg(ap, int);

					while (--field_width > 0) {
						*dest++ = ' ';
					}
				}

				format++;
				break;
			case 's':
				s = va_arg(ap, const char *);
				len = strnlen(s, precision);

				/* Right justification? */
				if ((flags & LEFT) == 0) {
					while (field_width-- > len) {
						*dest++ = ' ';
					}

					for (i = 0; i < len; i++) {
						*dest++ = *s++;
					}
				} else {
					for (i = 0; i < len; i++) {
						*dest++ = *s++;
					}

					while (field_width-- > len) {
						*dest++ = ' ';
					}
				}

				format++;
				break;
			case 'd':
			case 'i':
				dest += number(dest, va_arg(ap, int), 10, field_width, precision, flags | SIGNED);

				format++;
				break;
			case 'u':
				dest += number(dest, va_arg(ap, unsigned), 10, field_width, precision, flags);

				format++;
				break;
			case 'o':
				dest += number(dest, va_arg(ap, unsigned), 8, field_width, precision, flags);

				format++;
				break;
			case 'p':
				if (field_width == -1) {
					field_width = 2 * sizeof(void *);
					flags |= ZERO_PAD;
				}

				dest += number(dest, (unsigned long) va_arg(ap, void *), 16, field_width, precision, flags | POINTER);

				format++;
				break;
			case 'x':
				dest += number(dest, va_arg(ap, unsigned), 16, field_width, precision, flags);

				format++;
				break;
			case 'X':
				dest += number(dest, va_arg(ap, unsigned), 16, field_width, precision, flags | CAPITAL);

				format++;
				break;
			case '%':
				*dest++ = *format++;
				break;
			case 0:
				*dest++ = '%';
				*dest = 0;
				return dest - str;
			default:
				*dest++ = '%';
				*dest++ = *format++;
				break;
		}
	}

	*dest = 0;

	return dest - str;
}
