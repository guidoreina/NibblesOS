#include "kernel/keyboard.h"
#include "kernel/io.h"

#define EXTENDED_KEY 224

static void circular_queue_init(void);
static void circular_queue_push(unsigned char scancode);
static int circular_queue_pop(void);

void keyboard_init(void)
{
	circular_queue_init();
}

void keyboard_irq(void)
{
	static int extended_key = 0;
	unsigned char scancode;

	/* Read scan code. */
	scancode = inb(0x60);

	/* Extended key? */
	if (scancode == EXTENDED_KEY) {
		extended_key = 0x80;
		return;
	} else if ((scancode & 0x80) == 0) {
		/* Key pressed. */
		circular_queue_push(scancode | extended_key);
	}

	extended_key = 0;
}

int keyboard_get_key(void)
{
	return circular_queue_pop();
}

int keyboard_wait_for_key(void)
{
	int key;

	while ((key = circular_queue_pop()) < 0) {
		asm("hlt");
	}

	return key;
}

/*******************************************************************************
 *******************************************************************************
 **                                                                           **
 ** Circular queue of scancodes.                                              **
 **                                                                           **
 *******************************************************************************
 *******************************************************************************/
#define MAX_BUFFERED_SCANCODES 32

typedef struct {
	unsigned char scancodes[MAX_BUFFERED_SCANCODES];
	unsigned count;

	int first;
	int last;
} circular_queue_t;

static circular_queue_t cq;

void circular_queue_init(void)
{
	cq.count = 0;
	cq.first = -1;
	cq.last = -1;
}

void circular_queue_push(unsigned char scancode)
{
	/* If the queue is empty... */
	if (cq.count == 0) {
		cq.scancodes[0] = scancode;
		cq.first = 0;
		cq.last = 0;

		cq.count = 1;
	} else {
		cq.last = (cq.last + 1) % MAX_BUFFERED_SCANCODES;
		cq.scancodes[cq.last] = scancode;

		/* If the queue is full... */
		if (cq.count == MAX_BUFFERED_SCANCODES) {
			cq.first = (cq.first + 1) % MAX_BUFFERED_SCANCODES;
		} else {
			cq.count++;
		}
	}
}

int circular_queue_pop(void)
{
	unsigned char scancode;

	/* If the queue is empty... */
	if (cq.count == 0) {
		return -1;
	}

	scancode = cq.scancodes[cq.first];

	cq.first = (cq.first + 1) % MAX_BUFFERED_SCANCODES;

	cq.count--;

	return scancode;
}
