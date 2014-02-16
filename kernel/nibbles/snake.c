#include "kernel/stdlib.h"
#include "kernel/string.h"
#include "kernel/printf.h"
#include "kernel/curses.h"
#include "kernel/nibbles/snake.h"
#include "kernel/nibbles/field.h"
#include "kernel/nibbles/colors.h"

#define SNAKE_INITIAL_LENGTH 2

static void create_head(snake_t *snake, unsigned short x, unsigned short y);

void snake_init(snake_t *snake, const char *name, unsigned number, unsigned char color)
{
	size_t len;

	if ((len = strlen(name)) > SNAKE_NAME_MAX_LEN) {
		len = SNAKE_NAME_MAX_LEN;
	}

	memcpy(snake->name, name, len);
	snake->name[len] = 0;

	snake->number = number;

	snake->color = color;
}

void snake_reset(snake_t *snake)
{
	snake->head = NULL;
	snake->tail = NULL;

	snake->length = 0;

	create_head(snake, snake->initial_x, snake->initial_y);

	snake->grow = SNAKE_INITIAL_LENGTH - 1;

	snake->dir = snake->initial_dir;
}

void snake_move(snake_t *snake, unsigned short x, unsigned short y)
{
	/* If the snake is not growing... */
	if (snake->grow == 0) {
		snake->tail->x = x;
		snake->tail->y = y;

		if (snake->head != snake->tail) {
			snake->tail->prev->next = NULL;
			snake->tail->next = snake->head;
			snake->head->prev = snake->tail;
			snake->head = snake->tail;
			snake->tail = snake->tail->prev;
			snake->head->prev = NULL;
		}
	} else {
		create_head(snake, x, y);
		snake->grow--;
	}

	/* Show the new head. */
	set_point(y, x, snake->color);
}

void snake_grow(snake_t *snake, unsigned count)
{
	unsigned grow;
	unsigned left;

	grow = count * 4;

	if ((left = SNAKE_MAX_LENGTH - snake->length) < grow) {
		grow = left;
	}

	snake->grow += grow;
	snake->score += (count * 100);

	snake_show_score(snake);
}

void snake_die(snake_t *snake)
{
	snake->nlives--;
	snake->score -= 1000;

	snake_show_score(snake);
}

void snake_show(const snake_t *snake)
{
	const node_t *n = snake->head;
	while (n) {
		set_point(n->y, n->x, snake->color);
		n = n->next;
	}
}

void snake_hide(const snake_t *snake)
{
	const node_t *n = snake->head;
	while (n) {
		set_point(n->y, n->x, BACKGROUND_COLOR);
		n = n->next;
	}
}

void snake_hide_tail(const snake_t *snake)
{
	if (snake->grow == 0) {
		set_point(snake->tail->y, snake->tail->x, BACKGROUND_COLOR);
	}
}

void snake_show_score(const snake_t *snake)
{
	char msg[20];
	size_t len;

	len = sprintf(msg, "%s: %u, %d", snake->name, snake->nlives, snake->score);
	memset(msg + len, ' ', sizeof(msg) - len);

	attrset(A_FG_COLOR(MESSAGE_FOREGROUND_COLOR) | A_BG_COLOR(BACKGROUND_COLOR));
	mvaddnstr(0, snake->number * 20, msg, sizeof(msg));
}

void create_head(snake_t *snake, unsigned short x, unsigned short y)
{
	node_t *n = &snake->nodes[snake->length++];

	n->x = x;
	n->y = y;

	n->prev = NULL;
	n->next = snake->head;

	if (!snake->tail) {
		snake->tail = n;
	} else {
		snake->head->prev = n;
	}

	snake->head = n;
}
