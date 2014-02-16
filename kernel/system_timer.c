#include "kernel/system_timer.h"
#include "kernel/io.h"

/* http://tldp.org/HOWTO/KernelAnalysis-HOWTO-6.html */

#define HZ              100 /* Period: 10 ms. */
#define CLOCK_TICK_RATE 1193180
#define LATCH           ((CLOCK_TICK_RATE + HZ / 2) / HZ)

static unsigned ticks = 0;

void system_timer_init(void)
{
	/* Set control word.
	   BCD: Binary Counter (16 bits).
	   Mode 2: Rate Generator.
	   Read / Write least significant byte first,
	   then most significant byte.
	   Select Counter 0. */
	outb(0x43, 0x34);

	/* Set initial count. */
	outb(0x40, LATCH & 0xff);
	outb(0x40, LATCH >> 8);
}

void system_timer_irq(void)
{
	ticks++;
}

void system_timer_reset_ticks(void)
{
	ticks = 0;
}

unsigned system_timer_ticks(void)
{
	return ticks;
}
