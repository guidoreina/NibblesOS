#include "kernel/curses.h"
#include "kernel/nibbles/field.h"
#include "kernel/nibbles/colors.h"

void set_point(int y, int x, unsigned char color)
{
	int top;
	chtype oldch;
	int oldfg;
	int oldbg;
	char c;
	int fg;
	int bg;

	top = y % 2;

	/* Convert to screen coordinates. */
	y = (y - 1) / 2;
	x--;

	/* Get character at position. */
	oldch = mvinch(y, x);
	oldfg = FG_COLOR(oldch);
	oldbg = BG_COLOR(oldch);

	if (top) {
		/* Get bottom color. */
		int bottom_color;
		switch (oldch & 0xff) {
			case 0xdf:
				bottom_color = oldbg;
				break;
			case 0xdc:
			case 0xdb:
				bottom_color = oldfg;
				break;
			default:
				bottom_color = BACKGROUND_COLOR;
		}

		if (color == bottom_color) {
			c = (char) 0xdb; /* Top and bottom. */
			fg = color;
			bg = color;
		} else {
			c = (char) 0xdf; /* Top. */
			fg = color;
			bg = bottom_color;
		}
	} else {
		/* Get top color. */
		int top_color;
		switch (oldch & 0xff) {
			case 0xdc:
				top_color = oldbg;
				break;
			case 0xdf:
			case 0xdb:
				top_color = oldfg;
				break;
			default:
				top_color = BACKGROUND_COLOR;
		}

		if (color == top_color) {
			c = (char) 0xdb; /* Top and bottom. */
			fg = color;
			bg = color;
		} else {
			c = (char) 0xdc; /* Bottom. */
			fg = color;
			bg = top_color;
		}
	}

	mvaddch(y, x, MAKE_CHARACTER(c, fg, bg));
}

int point_is_free(int y, int x)
{
	int top;
	chtype oldch;
	int oldfg;
	int oldbg;

	top = y % 2;

	/* Convert to screen coordinates. */
	y = (y - 1) / 2;
	x--;

	/* Get character at position. */
	oldch = mvinch(y, x);
	oldfg = FG_COLOR(oldch);
	oldbg = BG_COLOR(oldch);

	if (top) {
		switch (oldch & 0xff) {
			case 0xdc:
				return (oldbg == BACKGROUND_COLOR);
			case 0xdf:
			case 0xdb:
				return (oldfg == BACKGROUND_COLOR);
			default:
				return 1;
		}
	} else {
		switch (oldch & 0xff) {
			case 0xdf:
				return (oldbg == BACKGROUND_COLOR);
			case 0xdc:
			case 0xdb:
				return (oldfg == BACKGROUND_COLOR);
			default:
				return 1;
		}
	}
}
