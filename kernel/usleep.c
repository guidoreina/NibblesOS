#include "kernel/usleep.h"
#include "kernel/system_timer.h"

void usleep(unsigned usec)
{
	unsigned msec = usec / 1000;

	system_timer_reset_ticks();

	while (system_timer_ticks() * 10 < msec) {
		asm("hlt");
	}
}
