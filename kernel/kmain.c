#include "kernel/idt.h"
#include "kernel/system_timer.h"
#include "kernel/keyboard.h"
#include "kernel/nibbles/nibbles.h"

int kmain()
{
	idt_init();
	system_timer_init();
	keyboard_init();

	/* Enable interrupts. */
	asm("sti");

	/* Play nibbles. */
	nibbles_play();

	/* Halt. */
	do {
		asm("hlt");
	} while (1);

	return 0;
}
