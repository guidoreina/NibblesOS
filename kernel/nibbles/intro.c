#include "kernel/keyboard.h"
#include "kernel/curses.h"
#include "kernel/usleep.h"
#include "kernel/nibbles/intro.h"

static void sparkle_pause(void);

void intro(void)
{
	clear();

	attrset(A_FG_COLOR(COLOR_WHITE) | A_BG_COLOR(COLOR_BLACK));
	mvaddstr(2, 34, "NibblesOS");

	attrset(A_FG_COLOR(COLOR_LIGHT_GRAY) | A_BG_COLOR(COLOR_BLACK));

	mvaddstr(4, 6, "NibblesOS is a game for one to four players.  Navigate your snakes");
	mvaddstr(5, 8, "around the game board trying to eat up numbers while avoiding");
	mvaddstr(6, 6, "running into walls or other snakes.  The more numbers you eat up,");
	mvaddstr(7, 9, "the more points you gain and the longer your snake becomes.");
	mvaddstr(9, 32, "Game Controls");
	mvaddstr(11, 35, "General");
	mvaddstr(12, 34, "P - Pause");
	mvaddstr(14, 6, "Player 1            Player 2            Player 3            Player 4");
	mvaddstr(15, 66, "(keypad)");
	mvaddstr(16, 8, "(Up)                (Up)                (Up)                (Up)");
	mvaddstr(17, 9, "\x18                   W                   O                   \x18");
	mvaddstr(18, 1, "(Left) \x1b \x1a (Right)  (Left) A D (Right)  (Left) K ; (Right)  (Left) \x1b \x1a (Right)");
	mvaddstr(19, 9, "\x19                   S                   L                   \x19");
	mvaddstr(20, 7, "(Down)              (Down)              (Down)              (Down)");
	mvaddstr(24, 26, "Press any key to continue");

	sparkle_pause();
}

void sparkle_pause(void)
{
	static const char *str = "*    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    ";
	unsigned a, b;

	attrset(A_FG_COLOR(COLOR_RED) | A_BG_COLOR(COLOR_BLACK));

	do {
		for (a = 0; a < 5; a++) {
			mvaddnstr(0, 0, str + a, 80);
			mvaddnstr(21, 0, str + (4 - a), 80);

			for (b = 1; b < 21; b++) {
				if ((a + b + 2) % 5 == 1) {
					mvaddch(b, 79, MAKE_CHARACTER('*', COLOR_RED, COLOR_BLACK));
					mvaddch(21 - b, 0, MAKE_CHARACTER('*', COLOR_RED, COLOR_BLACK));
				} else {
					mvaddch(b, 79, MAKE_CHARACTER(0, COLOR_BLACK, COLOR_BLACK));
					mvaddch(21 - b, 0, MAKE_CHARACTER(0, COLOR_BLACK, COLOR_BLACK));
				}
			}

			if (keyboard_get_key() != -1) {
				return;
			}

			usleep(150 * 1000);
		}
	} while (1);
}
