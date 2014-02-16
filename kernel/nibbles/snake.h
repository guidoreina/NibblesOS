#ifndef KERNEL_NIBBLES_SNAKE_H
#define KERNEL_NIBBLES_SNAKE_H

#define SNAKE_NAME_MAX_LEN 8
#define SNAKE_MAX_LENGTH   500

typedef enum {
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
} direction_t;

typedef struct node_t {
	unsigned short x;
	unsigned short y;

	struct node_t *prev;
	struct node_t *next;
} node_t;

typedef struct {
	char name[SNAKE_NAME_MAX_LEN + 1];
	unsigned number;

	node_t nodes[SNAKE_MAX_LENGTH];
	node_t *head;
	node_t *tail;

	unsigned length;
	unsigned grow;

	unsigned short initial_x;
	unsigned short initial_y;

	direction_t initial_dir;
	direction_t dir;

	unsigned nlives;
	int score;

	unsigned char color;
} snake_t;

void snake_init(snake_t *snake, const char *name, unsigned number, unsigned char color);
void snake_reset(snake_t *snake);
void snake_move(snake_t *snake, unsigned short x, unsigned short y);
void snake_grow(snake_t *snake, unsigned count);
void snake_die(snake_t *snake);
void snake_show(const snake_t *snake);
void snake_hide(const snake_t *snake);
void snake_hide_tail(const snake_t *snake);
void snake_show_score(const snake_t *snake);

#endif /* KERNEL_NIBBLES_SNAKE_H */
