#include "kernel/curses.h"
#include "kernel/string.h"
#include "kernel/printf.h"
#include "kernel/io.h"
#include "kernel/stdarg.h"

#define VIDEO ((unsigned char *) 0xb8000)

static unsigned char attributes = A_NORMAL;

void clear(void)
{
	memset(VIDEO, 0, 80 * 25 * 2);

	attributes = A_NORMAL;
}

void curs_set(int visibility)
{
	outb(0x3d4, 0x0a); /* Cursor start register. */
	outb(0x3d5, visibility ? 0x1f : 0x20);
}

void attrset(int attrs)
{
	attributes = (attrs & 0xff);
}

void mvaddch(int y, int x, const chtype ch)
{
	unsigned char *dest;

	dest = VIDEO + (80 * y * 2) + (x * 2);

	*dest++ = ch & 0x00ff;
	*dest = (ch & 0xff00) >> 8;
}

chtype mvinch(int y, int x)
{
	const unsigned char *src;

	src = VIDEO + (80 * y * 2) + (x * 2);

	return (src[0] | (src[1] << 8));
}

void mvaddstr(int y, int x, const char *str)
{
	unsigned char *dest;

	dest = VIDEO + (80 * y * 2) + (x * 2);

	while (*str) {
		*dest++ = *str++;
		*dest++ = attributes;
	}
}

void mvaddnstr(int y, int x, const char *str, int n)
{
	unsigned char *dest;
	int i;

	dest = VIDEO + (80 * y * 2) + (x * 2);

	for (i = 0; i < n; i++) {
		*dest++ = *str++;
		*dest++ = attributes;
	}
}

void mvprintw(int y, int x, const char *fmt, ...)
{
	char str[1024];
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = vsprintf(str, fmt, ap);
	va_end(ap);

	mvaddnstr(y, x, str, n);
}

void mvrect(int y, int x, int h, int w, chtype ch)
{
	unsigned char *dest;
	unsigned char c;
	unsigned char attr;
	int right, bottom;
	int j;

	c = ch & 0x00ff;
	attr = (ch & 0xff00) >> 8;

	for (bottom = y + h - 1, right = x + w - 1; y <= bottom; y++) {
		dest = VIDEO + (80 * y * 2) + (x * 2);

		for (j = x; j <= right; j++) {
			*dest++ = c;
			*dest++ = attr;
		}
	}
}

void mvhline(int y, int x, chtype ch, int n)
{
	unsigned char *dest;
	unsigned char c;
	unsigned char attr;
	int right;

	dest = VIDEO + (80 * y * 2) + (x * 2);

	c = ch & 0x00ff;
	attr = (ch & 0xff00) >> 8;

	for (right = x + n - 1; x <= right; x++) {
		*dest++ = c;
		*dest++ = attr;
	}
}

void mvvline(int y, int x, chtype ch, int n)
{
	unsigned char *dest;
	unsigned char c;
	unsigned char attr;
	int bottom;

	c = ch & 0x00ff;
	attr = (ch & 0xff00) >> 8;

	for (bottom = y + n - 1; y <= bottom; y++) {
		dest = VIDEO + (80 * y * 2) + (x * 2);

		*dest++ = c;
		*dest++ = attr;
	}
}
