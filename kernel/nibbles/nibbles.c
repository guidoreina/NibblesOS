#include "kernel/keyboard.h"
#include "kernel/curses.h"
#include "kernel/printf.h"
#include "kernel/usleep.h"
#include "kernel/random.h"
#include "kernel/types.h"
#include "kernel/nibbles/nibbles.h"
#include "kernel/nibbles/snake.h"
#include "kernel/nibbles/intro.h"
#include "kernel/nibbles/inputs.h"
#include "kernel/nibbles/field.h"
#include "kernel/nibbles/colors.h"

#define MAX_SNAKES 4

static void play(void);
static void draw_screen(void);
static void prepare_level(unsigned level);
static void hide_snakes(void);
static void hide_snakes_tails(void);
static void show_scores(void);
static void reset_snakes(void);
static void process_events(void);
static void move_snakes(void);
static void kill_snake(snake_t *snake);
static void place_number(char number);
static void hide_number(void);
static void show_message(const char *msg, size_t len);
static void show_snake_dies(const snake_t *snake);
static int play_again(void);

static snake_t snakes[MAX_SNAKES];
static unsigned nsnakes;
static unsigned level;
static unsigned speed;
static int increase_speed;
static char number;
static unsigned number_x;
static unsigned number_y;
static int game_over;

void nibbles_play(void)
{
	/* Set snakes' names and colors. */
	snake_init(&snakes[0], "Sammy", 0, PLAYER_1_COLOR);
	snake_init(&snakes[1], "Jake", 1, PLAYER_2_COLOR);
	snake_init(&snakes[2], "Tom", 2, PLAYER_3_COLOR);
	snake_init(&snakes[3], "Jerry", 3, PLAYER_4_COLOR);

	/* Disable cursor. */
	curs_set(0);

	do {
		intro();

		get_inputs(&nsnakes, &speed, &increase_speed);

		play();
	} while (play_again());

	/* Clear screen. */
	clear();
}

void play(void)
{
	unsigned i;

	/* Reset snakes' number of lives and scores. */
	for (i = 0; i < nsnakes; i++) {
		snakes[i].nlives = 5;
		snakes[i].score = 0;
	}

	/* Prepare level. */
	level = 1;
	prepare_level(level);

	game_over = 0;

	do {
		usleep(speed * 1000);

		process_events();

		move_snakes();
	} while (!game_over);
}

void draw_screen(void)
{
	mvrect(0, 0, 25, 80, MAKE_CHARACTER(0xdb, BACKGROUND_COLOR, BACKGROUND_COLOR));

	mvhline(1, 1, MAKE_CHARACTER(0xdf, WALL_COLOR, BACKGROUND_COLOR), 78);
	mvhline(24, 1, MAKE_CHARACTER(0xdc, WALL_COLOR, BACKGROUND_COLOR), 78);

	mvvline(1, 0, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 24);
	mvvline(1, 79, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 24);
}

void prepare_level(unsigned level)
{
	char msg[64];
	size_t len;
	unsigned i;

	draw_screen();
	show_scores();

	switch (level) {
		case 1:
			snakes[0].initial_y = 25; snakes[0].initial_x = 50; snakes[0].initial_dir = DIR_RIGHT;
			snakes[1].initial_y = 25; snakes[1].initial_x = 30; snakes[1].initial_dir = DIR_LEFT;
			snakes[2].initial_y = 27; snakes[2].initial_x = 40; snakes[2].initial_dir = DIR_DOWN;
			snakes[3].initial_y = 23; snakes[3].initial_x = 40; snakes[3].initial_dir = DIR_UP;
			break;
		case 2:
			mvhline(12, 19, MAKE_CHARACTER(0xdf, WALL_COLOR, BACKGROUND_COLOR), 41);

			snakes[0].initial_y = 7; snakes[0].initial_x = 60; snakes[0].initial_dir = DIR_LEFT;
			snakes[1].initial_y = 43; snakes[1].initial_x = 20; snakes[1].initial_dir = DIR_RIGHT;
			snakes[2].initial_y = 40; snakes[2].initial_x = 10; snakes[2].initial_dir = DIR_UP;
			snakes[3].initial_y = 10; snakes[3].initial_x = 70; snakes[3].initial_dir = DIR_DOWN;
			break;
		case 3:
			mvaddch(4, 19, MAKE_CHARACTER(0xdc, WALL_COLOR, BACKGROUND_COLOR));
			mvaddch(4, 59, MAKE_CHARACTER(0xdc, WALL_COLOR, BACKGROUND_COLOR));
			mvvline(5, 19, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 15);
			mvvline(5, 59, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 15);

			snakes[0].initial_y = 25; snakes[0].initial_x = 50; snakes[0].initial_dir = DIR_UP;
			snakes[1].initial_y = 25; snakes[1].initial_x = 30; snakes[1].initial_dir = DIR_DOWN;
			snakes[2].initial_y = 7; snakes[2].initial_x = 57; snakes[2].initial_dir = DIR_LEFT;
			snakes[3].initial_y = 43; snakes[3].initial_x = 21; snakes[3].initial_dir = DIR_RIGHT;
			break;
		case 4:
			mvhline(7, 40, MAKE_CHARACTER(0xdf, WALL_COLOR, BACKGROUND_COLOR), 39);
			mvhline(18, 1, MAKE_CHARACTER(0xdc, WALL_COLOR, BACKGROUND_COLOR), 39);
			mvvline(1, 19, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 14);
			mvvline(11, 59, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 14);

			snakes[0].initial_y = 7; snakes[0].initial_x = 60; snakes[0].initial_dir = DIR_LEFT;
			snakes[1].initial_y = 43; snakes[1].initial_x = 20; snakes[1].initial_dir = DIR_RIGHT;
			snakes[2].initial_y = 30; snakes[2].initial_x = 9; snakes[2].initial_dir = DIR_UP;
			snakes[3].initial_y = 20; snakes[3].initial_x = 70; snakes[3].initial_dir = DIR_DOWN;
			break;
		case 5:
			mvhline(5, 22, MAKE_CHARACTER(0xdf, WALL_COLOR, BACKGROUND_COLOR), 35);
			mvhline(20, 22, MAKE_CHARACTER(0xdf, WALL_COLOR, BACKGROUND_COLOR), 35);
			mvvline(6, 20, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 13);
			mvvline(6, 58, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 13);
			mvaddch(19, 20, MAKE_CHARACTER(0xdf, WALL_COLOR, BACKGROUND_COLOR));
			mvaddch(19, 58, MAKE_CHARACTER(0xdf, WALL_COLOR, BACKGROUND_COLOR));

			snakes[0].initial_y = 25; snakes[0].initial_x = 50; snakes[0].initial_dir = DIR_UP;
			snakes[1].initial_y = 25; snakes[1].initial_x = 30; snakes[1].initial_dir = DIR_DOWN;
			snakes[2].initial_y = 15; snakes[2].initial_x = 55; snakes[2].initial_dir = DIR_LEFT;
			snakes[3].initial_y = 35; snakes[3].initial_x = 23; snakes[3].initial_dir = DIR_RIGHT;
			break;
		case 6:
			mvvline(1, 9, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 10);
			mvvline(1, 19, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 10);
			mvvline(1, 29, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 10);
			mvvline(1, 39, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 10);
			mvvline(1, 49, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 10);
			mvvline(1, 59, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 10);
			mvvline(1, 69, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 10);

			mvvline(15, 9, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 10);
			mvvline(15, 19, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 10);
			mvvline(15, 29, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 10);
			mvvline(15, 39, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 10);
			mvvline(15, 49, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 10);
			mvvline(15, 59, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 10);
			mvvline(15, 69, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 10);

			snakes[0].initial_y = 7; snakes[0].initial_x = 65; snakes[0].initial_dir = DIR_DOWN;
			snakes[1].initial_y = 43; snakes[1].initial_x = 15; snakes[1].initial_dir = DIR_UP;
			snakes[2].initial_y = 7; snakes[2].initial_x = 25; snakes[2].initial_dir = DIR_DOWN;
			snakes[3].initial_y = 43; snakes[3].initial_x = 55; snakes[3].initial_dir = DIR_UP;
			break;
		case 7:
			mvaddch(1, 39, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR));
			mvvline(2, 39, MAKE_CHARACTER(0xdc, WALL_COLOR, BACKGROUND_COLOR), 22);

			snakes[0].initial_y = 7; snakes[0].initial_x = 65; snakes[0].initial_dir = DIR_DOWN;
			snakes[1].initial_y = 43; snakes[1].initial_x = 15; snakes[1].initial_dir = DIR_UP;
			snakes[2].initial_y = 43; snakes[2].initial_x = 44; snakes[2].initial_dir = DIR_UP;
			snakes[3].initial_y = 7; snakes[3].initial_x = 34; snakes[3].initial_dir = DIR_DOWN;
			break;
		case 8:
			mvvline(1, 9, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 19);
			mvvline(6, 19, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 19);
			mvvline(1, 29, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 19);
			mvvline(6, 39, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 19);
			mvvline(1, 49, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 19);
			mvvline(6, 59, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 19);
			mvvline(1, 69, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR), 19);

			snakes[0].initial_y = 7; snakes[0].initial_x = 65; snakes[0].initial_dir = DIR_DOWN;
			snakes[1].initial_y = 43; snakes[1].initial_x = 15; snakes[1].initial_dir = DIR_UP;
			snakes[2].initial_y = 7; snakes[2].initial_x = 25; snakes[2].initial_dir = DIR_DOWN;
			snakes[3].initial_y = 43; snakes[3].initial_x = 55; snakes[3].initial_dir = DIR_UP;
			break;
		case 9:
			for (i = 6; i <= 47; i++) {
				int y = (i - 1) / 2;
				char c = ((i % 2) == 0) ? 0xdc : 0xdf;

				mvaddch(y, i - 1, MAKE_CHARACTER(c, WALL_COLOR, BACKGROUND_COLOR));
				mvaddch(y, i + 27, MAKE_CHARACTER(c, WALL_COLOR, BACKGROUND_COLOR));
			}

			snakes[0].initial_y = 40; snakes[0].initial_x = 75; snakes[0].initial_dir = DIR_UP;
			snakes[1].initial_y = 15; snakes[1].initial_x = 5; snakes[1].initial_dir = DIR_DOWN;
			snakes[2].initial_y = 5; snakes[2].initial_x = 65; snakes[2].initial_dir = DIR_DOWN;
			snakes[3].initial_y = 45; snakes[3].initial_x = 15; snakes[3].initial_dir = DIR_UP;
			break;
		default:
			mvaddch(1, 9, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR));
			mvvline(2, 9, MAKE_CHARACTER(0xdc, WALL_COLOR, BACKGROUND_COLOR), 22);
			mvvline(2, 19, MAKE_CHARACTER(0xdf, WALL_COLOR, BACKGROUND_COLOR), 22);
			mvaddch(24, 19, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR));
			mvaddch(1, 29, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR));
			mvvline(2, 29, MAKE_CHARACTER(0xdc, WALL_COLOR, BACKGROUND_COLOR), 22);
			mvvline(2, 39, MAKE_CHARACTER(0xdf, WALL_COLOR, BACKGROUND_COLOR), 22);
			mvaddch(24, 39, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR));
			mvaddch(1, 49, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR));
			mvvline(2, 49, MAKE_CHARACTER(0xdc, WALL_COLOR, BACKGROUND_COLOR), 22);
			mvvline(2, 59, MAKE_CHARACTER(0xdf, WALL_COLOR, BACKGROUND_COLOR), 22);
			mvaddch(24, 59, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR));
			mvaddch(1, 69, MAKE_CHARACTER(0xdb, WALL_COLOR, BACKGROUND_COLOR));
			mvvline(2, 69, MAKE_CHARACTER(0xdc, WALL_COLOR, BACKGROUND_COLOR), 22);

			snakes[0].initial_y = 7; snakes[0].initial_x = 65; snakes[0].initial_dir = DIR_DOWN;
			snakes[1].initial_y = 43; snakes[1].initial_x = 15; snakes[1].initial_dir = DIR_UP;
			snakes[2].initial_y = 43; snakes[2].initial_x = 55; snakes[2].initial_dir = DIR_UP;
			snakes[3].initial_y = 7; snakes[3].initial_x = 25; snakes[3].initial_dir = DIR_DOWN;
	}

	len = sprintf(msg, "      Level %u,  Push Space     ", level);
	show_message(msg, len);

	reset_snakes();

	number = '1';
	place_number(number);
}

void hide_snakes(void)
{
	unsigned i;

	for (i = 0; i < nsnakes; i++) {
		snake_hide(&snakes[i]);
	}
}

void hide_snakes_tails(void)
{
	unsigned i;

	for (i = 0; i < nsnakes; i++) {
		snake_hide_tail(&snakes[i]);
	}
}

void show_scores(void)
{
	unsigned i;

	for (i = 0; i < nsnakes; i++) {
		snake_show_score(&snakes[i]);
	}
}

void reset_snakes(void)
{
	unsigned i;

	for (i = 0; i < nsnakes; i++) {
		snake_reset(&snakes[i]);
		snake_show(&snakes[i]);
	}
}

void process_events(void)
{
	int has_event[MAX_SNAKES];
	unsigned i;
	int key;

	for (i = 0; i < MAX_SNAKES; i++) {
		has_event[i] = 0;
	}

	while ((key = keyboard_get_key()) != -1) {
		switch (key) {
			case SCANCODE_UP:
				if ((!has_event[0]) && (snakes[0].dir != DIR_DOWN)) {
					snakes[0].dir = DIR_UP;
					has_event[0] = 1;
				}

				break;
			case SCANCODE_DOWN:
				if ((!has_event[0]) && (snakes[0].dir != DIR_UP)) {
					snakes[0].dir = DIR_DOWN;
					has_event[0] = 1;
				}

				break;
			case SCANCODE_LEFT:
				if ((!has_event[0]) && (snakes[0].dir != DIR_RIGHT)) {
					snakes[0].dir = DIR_LEFT;
					has_event[0] = 1;
				}

				break;
			case SCANCODE_RIGHT:
				if ((!has_event[0]) && (snakes[0].dir != DIR_LEFT)) {
					snakes[0].dir = DIR_RIGHT;
					has_event[0] = 1;
				}

				break;
			case SCANCODE_W:
				if ((!has_event[1]) && (snakes[1].dir != DIR_DOWN)) {
					snakes[1].dir = DIR_UP;
					has_event[1] = 1;
				}

				break;
			case SCANCODE_S:
				if ((!has_event[1]) && (snakes[1].dir != DIR_UP)) {
					snakes[1].dir = DIR_DOWN;
					has_event[1] = 1;
				}

				break;
			case SCANCODE_A:
				if ((!has_event[1]) && (snakes[1].dir != DIR_RIGHT)) {
					snakes[1].dir = DIR_LEFT;
					has_event[1] = 1;
				}

				break;
			case SCANCODE_D:
				if ((!has_event[1]) && (snakes[1].dir != DIR_LEFT)) {
					snakes[1].dir = DIR_RIGHT;
					has_event[1] = 1;
				}

				break;
			case SCANCODE_O:
				if ((!has_event[2]) && (snakes[2].dir != DIR_DOWN)) {
					snakes[2].dir = DIR_UP;
					has_event[2] = 1;
				}

				break;
			case SCANCODE_L:
				if ((!has_event[2]) && (snakes[2].dir != DIR_UP)) {
					snakes[2].dir = DIR_DOWN;
					has_event[2] = 1;
				}

				break;
			case SCANCODE_K:
				if ((!has_event[2]) && (snakes[2].dir != DIR_RIGHT)) {
					snakes[2].dir = DIR_LEFT;
					has_event[2] = 1;
				}

				break;
			case SCANCODE_SEMICOLON:
				if ((!has_event[2]) && (snakes[2].dir != DIR_LEFT)) {
					snakes[2].dir = DIR_RIGHT;
					has_event[2] = 1;
				}

				break;
			case SCANCODE_KEYPAD_UP:
				if ((!has_event[3]) && (snakes[3].dir != DIR_DOWN)) {
					snakes[3].dir = DIR_UP;
					has_event[3] = 1;
				}

				break;
			case SCANCODE_KEYPAD_DOWN:
				if ((!has_event[3]) && (snakes[3].dir != DIR_UP)) {
					snakes[3].dir = DIR_DOWN;
					has_event[3] = 1;
				}

				break;
			case SCANCODE_KEYPAD_LEFT:
				if ((!has_event[3]) && (snakes[3].dir != DIR_RIGHT)) {
					snakes[3].dir = DIR_LEFT;
					has_event[3] = 1;
				}

				break;
			case SCANCODE_KEYPAD_RIGHT:
				if ((!has_event[3]) && (snakes[3].dir != DIR_LEFT)) {
					snakes[3].dir = DIR_RIGHT;
					has_event[3] = 1;
				}

				break;
			case SCANCODE_P: /* Pause. */
				show_message("  Game Paused ... Push Space   ", 31);
				break;
		}
	}
}

void move_snakes(void)
{
	unsigned short x[MAX_SNAKES];
	unsigned short y[MAX_SNAKES];
	int dead[MAX_SNAKES];
	int somebody_died;
	int number_eaten;
	snake_t *snake;
	unsigned i, j;

	/* Calculate the new position of the heads and whether the
	   snakes hit the number. */
	number_eaten = 0;
	for (i = 0; i < nsnakes; i++) {
		snake = &snakes[i];
		dead[i] = 0;

		switch (snake->dir) {
			case DIR_UP:
				x[i] = snake->head->x;
				y[i] = snake->head->y - 1;
				break;
			case DIR_DOWN:
				x[i] = snake->head->x;
				y[i] = snake->head->y + 1;
				break;
			case DIR_LEFT:
				x[i] = snake->head->x - 1;
				y[i] = snake->head->y;
				break;
			case DIR_RIGHT:
				x[i] = snake->head->x + 1;
				y[i] = snake->head->y;
				break;
		}

		if ((number_x == x[i]) && ((number_y == y[i]) || (number_y + 1 == y[i]))) {
			snake_grow(snake, number - '0');
			number_eaten = 1;
		}
	}

	/* Check head collisions. */
	somebody_died = 0;
	for (i = 0; i < nsnakes; i++) {
		/* If the snake has just died... */
		if (dead[i]) {
			continue;
		}

		for (j = 0; j < nsnakes; j++) {
			if (i == j) {
				continue;
			}

			/* Head collision? */
			if ((x[i] == x[j]) && (y[i] == y[j])) {
				/* Kill both snakes. */
				dead[i] = 1;
				dead[j] = 1;

				somebody_died = 1;
			}
		}
	}

	/* Check collisions. */
	hide_snakes_tails();
	for (i = 0; i < nsnakes; i++) {
		/* If the snake has just died... */
		if (dead[i]) {
			continue;
		}

		if (!point_is_free(y[i], x[i])) {
			dead[i] = 1;
			somebody_died = 1;
		}
	}

	/* If no snake has died... */
	if (!somebody_died) {
		if (number_eaten) {
			if (number == '9') {
				if (level == 10) {
					game_over = 1;
				} else {
					prepare_level(++level);

					if ((increase_speed) && (speed > 10)) {
						speed -= 10;
					}
				}
			} else {
				for (i = 0; i < nsnakes; i++) {
					/* Move snake. */
					snake_move(&snakes[i], x[i], y[i]);
				}

				place_number(++number);
			}
		} else {
			for (i = 0; i < nsnakes; i++) {
				/* Move snake. */
				snake_move(&snakes[i], x[i], y[i]);
			}
		}
	} else {
		hide_snakes();
		hide_number();

		for (i = 0; i < nsnakes; i++) {
			if (dead[i]) {
				kill_snake(&snakes[i]);
				show_snake_dies(&snakes[i]);
			}
		}

		if (!game_over) {
			reset_snakes();

			number = '1';
			place_number(number);
		}
	}
}

void kill_snake(snake_t *snake)
{
	snake_die(snake);

	if (snake->nlives == 0) {
		game_over = 1;
	}
}

void place_number(char number)
{
	do {
		number_x = (random() % 78) + 2;
		number_y = (random() % 44) + 5;

		/* Bottom? */
		if ((number_y % 2) == 0) {
			number_y--;
		}
	} while ((!point_is_free(number_y, number_x)) || (!point_is_free(number_y + 1, number_x)));

	mvaddch((number_y - 1) / 2, number_x - 1, MAKE_CHARACTER(number, NUMBER_COLOR, BACKGROUND_COLOR));
}

void hide_number(void)
{
	mvaddch((number_y - 1) / 2, number_x - 1, MAKE_CHARACTER(0xdb, BACKGROUND_COLOR, BACKGROUND_COLOR));
}

void show_message(const char *msg, size_t len)
{
	static const int y = ((25 - 3) / 2) - 1;
	static const int h = 3;
	static const int bottom = (((25 - 3) / 2) - 1) + 3 - 1;

	int x = (80 - (len + 2)) / 2;
	int w = len + 2;

	chtype rect[3][80];
	int i, j;
	int right = x + w - 1;

	/* Save screen's rectangle. */
	for (j = y; j <= bottom; j++) {
		for (i = x; i <= right; i++) {
			rect[j - y][i] = mvinch(j, i);
		}
	}

	mvhline(y, x + 1, MAKE_CHARACTER(0xdf, MESSAGE_BORDER_COLOR, MESSAGE_BACKGROUND_COLOR), w - 2);
	mvhline(bottom, x + 1, MAKE_CHARACTER(0xdc, MESSAGE_BORDER_COLOR, MESSAGE_BACKGROUND_COLOR), w - 2);

	mvvline(y, x, MAKE_CHARACTER(0xdb, MESSAGE_BORDER_COLOR, MESSAGE_BACKGROUND_COLOR), h);
	mvvline(y, right, MAKE_CHARACTER(0xdb, MESSAGE_BORDER_COLOR, MESSAGE_BACKGROUND_COLOR), h);

	attrset(A_FG_COLOR(MESSAGE_FOREGROUND_COLOR) | A_BG_COLOR(MESSAGE_BACKGROUND_COLOR));
	mvaddnstr(y + 1, x + 1, msg, len);

	/* Wait for SPACE key. */
	while (keyboard_wait_for_key() != SCANCODE_SPACE);

	/* Restore screen's rectangle. */
	for (j = y; j <= bottom; j++) {
		for (i = x; i <= right; i++) {
			mvaddch(j, i, rect[j - y][i]);
		}
	}
}

void show_snake_dies(const snake_t *snake)
{
	char msg[64];
	size_t len;

	if ((snake->number % 2) == 0) {
		len = sprintf(msg, "  %s Dies! Push Space! ---> ", snake->name);
	} else {
		len = sprintf(msg, "  <---- %s Dies! Push Space  ", snake->name);
	}

	show_message(msg, len);
}

int play_again(void)
{
	#define FIRST_LINE  "       G A M E   O V E R       "
	#define SECOND_LINE "      Play Again?   (Y/N)      "

	static const int y = ((25 - 5) / 2) - 1;
	static const int x = (80 - (sizeof(FIRST_LINE) + 1)) / 2;
	static const int h = 5;
	static const int w = sizeof(FIRST_LINE) + 1;

	int key;

	mvhline(y, x + 1, MAKE_CHARACTER(0xdf, MESSAGE_BORDER_COLOR, MESSAGE_BACKGROUND_COLOR), w - 2);
	mvhline(y + h - 1, x + 1, MAKE_CHARACTER(0xdc, MESSAGE_BORDER_COLOR, MESSAGE_BACKGROUND_COLOR), w - 2);

	mvvline(y, x, MAKE_CHARACTER(0xdb, MESSAGE_BORDER_COLOR, MESSAGE_BACKGROUND_COLOR), h);
	mvvline(y, x + w - 1, MAKE_CHARACTER(0xdb, MESSAGE_BORDER_COLOR, MESSAGE_BACKGROUND_COLOR), h);

	attrset(A_FG_COLOR(MESSAGE_FOREGROUND_COLOR) | A_BG_COLOR(MESSAGE_BACKGROUND_COLOR));
	mvaddnstr(y + 1, x + 1, FIRST_LINE, sizeof(FIRST_LINE) - 1);
	mvhline(y + 2, x + 1, MAKE_CHARACTER(0xdb, MESSAGE_BACKGROUND_COLOR, MESSAGE_BACKGROUND_COLOR), w - 2);
	mvaddnstr(y + 3, x + 1, SECOND_LINE, sizeof(SECOND_LINE) - 1);

	#undef FIRST_LINE
	#undef SECOND_LINE

	/* Wait for Y/N key. */
	while (((key = keyboard_wait_for_key()) != SCANCODE_Y) && (key != SCANCODE_N));

	return (key == SCANCODE_Y);
}
