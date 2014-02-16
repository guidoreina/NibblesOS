#include "kernel/keyboard.h"
#include "kernel/curses.h"
#include "kernel/nibbles/inputs.h"

static unsigned read_number(int y, int x, unsigned maxlen, unsigned min, unsigned max);
static int read_yes_no(int y, int x);

void get_inputs(unsigned *nsnakes, unsigned *speed, int *increase_speed)
{
	clear();

	mvaddstr(4, 19, "How many players (1 to 4)?");
	*nsnakes = read_number(4, 46, 1, 1, 4);

	mvaddstr(7, 20, "Skill level (1 to 100)?");
	mvaddstr(8, 21, "1   = Novice");
	mvaddstr(9, 21, "90  = Expert");
	mvaddstr(10, 21, "100 = Twiddle finger");
	mvaddstr(11, 14, "Computer speed may affect your skill level)");

	*speed = read_number(7, 44, 3, 1, 100);
	*speed = ((100 - *speed) * 2) + 1;

	mvaddstr(14, 14, "Increase game speed during play (Y or N)?");
	*increase_speed = read_yes_no(14, 56);
}

unsigned read_number(int y, int x, unsigned maxlen, unsigned min, unsigned max)
{
	unsigned n;
	unsigned len;
	unsigned c;
	unsigned i;
	int key;

	/* Show cursor. */
	mvaddch(y, x, MAKE_CHARACTER('_', COLOR_LIGHT_GRAY, COLOR_BLACK));

	n = 0;
	len = 0;

	do {
		switch ((key = keyboard_wait_for_key())) {
			case SCANCODE_1:
			case SCANCODE_2:
			case SCANCODE_3:
			case SCANCODE_4:
			case SCANCODE_5:
			case SCANCODE_6:
			case SCANCODE_7:
			case SCANCODE_8:
			case SCANCODE_9:
			case SCANCODE_0:
				if (len < maxlen) {
					c = ((key - SCANCODE_1) + 1) % 10;

					/* Show character. */
					mvaddch(y, x + len, MAKE_CHARACTER('0' + c, COLOR_LIGHT_GRAY, COLOR_BLACK));

					n = (n * 10) + c;
					len++;

					/* Show cursor. */
					mvaddch(y, x + len, MAKE_CHARACTER('_', COLOR_LIGHT_GRAY, COLOR_BLACK));
				}

				break;
			case SCANCODE_BACKSPACE:
				if (len > 0) {
					/* Hide cursor. */
					mvaddch(y, x + len, MAKE_CHARACTER(0, COLOR_BLACK, COLOR_BLACK));

					len--;

					/* Show cursor. */
					mvaddch(y, x + len, MAKE_CHARACTER('_', COLOR_LIGHT_GRAY, COLOR_BLACK));

					n /= 10;
				}

				break;
			case SCANCODE_ENTER:
				if (len > 0) {
					if ((n >= min) && (n <= max)) {
						/* Hide cursor. */
						mvaddch(y, x + len, MAKE_CHARACTER(0, COLOR_BLACK, COLOR_BLACK));

						return n;
					}

					/* Show cursor. */
					mvaddch(y, x, MAKE_CHARACTER('_', COLOR_LIGHT_GRAY, COLOR_BLACK));

					for (i = 1; i <= maxlen; i++) {
						mvaddch(y, x + i, MAKE_CHARACTER(0, COLOR_BLACK, COLOR_BLACK));
					}

					n = 0;
					len = 0;
				}

				break;
		}
	} while (1);
}

int read_yes_no(int y, int x)
{
	char c;

	/* Show cursor. */
	mvaddch(y, x, MAKE_CHARACTER('_', COLOR_LIGHT_GRAY, COLOR_BLACK));

	c = 0;

	do {
		switch (keyboard_wait_for_key()) {
			case SCANCODE_Y:
				if (c == 0) {
					c = 'Y';

					/* Show character. */
					mvaddch(y, x, MAKE_CHARACTER(c, COLOR_LIGHT_GRAY, COLOR_BLACK));

					/* Show cursor. */
					mvaddch(y, x + 1, MAKE_CHARACTER('_', COLOR_LIGHT_GRAY, COLOR_BLACK));
				}

				break;
			case SCANCODE_N:
				if (c == 0) {
					c = 'N';

					/* Show character. */
					mvaddch(y, x, MAKE_CHARACTER(c, COLOR_LIGHT_GRAY, COLOR_BLACK));

					/* Show cursor. */
					mvaddch(y, x + 1, MAKE_CHARACTER('_', COLOR_LIGHT_GRAY, COLOR_BLACK));
				}

				break;
			case SCANCODE_BACKSPACE:
				if (c != 0) {
					/* Hide cursor. */
					mvaddch(y, x + 1, MAKE_CHARACTER(0, COLOR_BLACK, COLOR_BLACK));

					/* Show cursor. */
					mvaddch(y, x, MAKE_CHARACTER('_', COLOR_LIGHT_GRAY, COLOR_BLACK));

					c = 0;
				}

				break;
			case SCANCODE_ENTER:
				if (c != 0) {
					/* Hide cursor. */
					mvaddch(y, x + 1, MAKE_CHARACTER(0, COLOR_BLACK, COLOR_BLACK));

					return (c == 'Y') ? 1 : 0;
				}

				break;
		}
	} while (1);
}
