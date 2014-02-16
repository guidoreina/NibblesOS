#include "kernel/irq_handler.h"
#include "kernel/io.h"
#include "kernel/system_timer.h"
#include "kernel/keyboard.h"

void irq_handler(unsigned number)
{
	switch (number) {
		case 0:
			system_timer_irq();
			break;
		case 1:
			keyboard_irq();
			break;
	}

	if (number >= 8) {
		/* Send End-Of-Interrupt to the second PIC. */
		outb(0xa0, 0x20);
	}

	/* Send End-Of-Interrupt to the first PIC. */
	outb(0x20, 0x20);
}
