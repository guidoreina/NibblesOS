#include "kernel/idt.h"
#include "kernel/exception_handler.h"
#include "kernel/irq_handler.h"
#include "kernel/interrupt_handler.h"
#include "kernel/string.h"
#include "kernel/types.h"

#define NUMBER_OF_DESCRIPTORS 256
#define NUMBER_OF_EXCEPTIONS  32
#define NUMBER_OF_IRQS        16
#define NUMBER_OF_INTERRUPTS  32

struct __attribute__((__packed__)) idt_descriptor {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t fixed;
	uint8_t flags;
	uint16_t offset_high;
};

typedef struct idt_descriptor idt_descriptor_t;

struct __attribute__((__packed__)) idt_register {
	uint16_t limit;
	uint32_t base;
};

typedef struct idt_register idt_register_t;

/* IDT. */
static idt_descriptor_t idt[NUMBER_OF_DESCRIPTORS];

/* IDTR. */
static idt_register_t idtr;
static idt_register_t *pidtr = &idtr;

static void set_idt_descriptor(idt_descriptor_t *desc, uint16_t selector, uint32_t offset, uint8_t flags);

void idt_init(void)
{
	static const exception_handler_t exception_handlers[NUMBER_OF_EXCEPTIONS] = {
		exception_handler0,  exception_handler1,  exception_handler2,  exception_handler3,
		exception_handler4,  exception_handler5,  exception_handler6,  exception_handler7,
		exception_handler8,  exception_handler9,  exception_handler10, exception_handler11,
		exception_handler12, exception_handler13, exception_handler14, exception_handler15,
		exception_handler16, exception_handler17, exception_handler18, exception_handler19,
		exception_handler20, exception_handler21, exception_handler22, exception_handler23,
		exception_handler24, exception_handler25, exception_handler26, exception_handler27,
		exception_handler28, exception_handler29, exception_handler30, exception_handler31
	};

	static const irq_handler_t irq_handlers[NUMBER_OF_IRQS] = {
		irq_handler0,  irq_handler1,  irq_handler2,  irq_handler3,
		irq_handler4,  irq_handler5,  irq_handler6,  irq_handler7,
		irq_handler8,  irq_handler9,  irq_handler10, irq_handler11,
		irq_handler12, irq_handler13, irq_handler14, irq_handler15
	};

	static const interrupt_handler_t interrupt_handlers[NUMBER_OF_INTERRUPTS] = {
		interrupt_handler0,  interrupt_handler1,  interrupt_handler2,  interrupt_handler3,
		interrupt_handler4,  interrupt_handler5,  interrupt_handler6,  interrupt_handler7,
		interrupt_handler8,  interrupt_handler9,  interrupt_handler10, interrupt_handler11,
		interrupt_handler12, interrupt_handler13, interrupt_handler14, interrupt_handler15,
		interrupt_handler16, interrupt_handler17, interrupt_handler18, interrupt_handler19,
		interrupt_handler20, interrupt_handler21, interrupt_handler22, interrupt_handler23,
		interrupt_handler24, interrupt_handler25, interrupt_handler26, interrupt_handler27,
		interrupt_handler28, interrupt_handler29, interrupt_handler30, interrupt_handler31
	};

	unsigned i;

	memset(idt, 0, sizeof(idt));

	/* Set exceptions (interrupt gate). */
	for (i = 0; i < NUMBER_OF_EXCEPTIONS; i++) {
		set_idt_descriptor(&idt[i], 8, (uint32_t) exception_handlers[i], 0x8e); /* 10001110b */
	}

	/* Set IRQs (interrupt gate). */
	for (i = 0; i < NUMBER_OF_IRQS; i++) {
		set_idt_descriptor(&idt[NUMBER_OF_EXCEPTIONS + i], 8, (uint32_t) irq_handlers[i], 0x8e); /* 10001110b */
	}

	/* Set interrupts (interrupt gate). */
	for (i = 0; i < NUMBER_OF_INTERRUPTS; i++) {
		set_idt_descriptor(&idt[NUMBER_OF_EXCEPTIONS + NUMBER_OF_IRQS + i], 8, (uint32_t) interrupt_handlers[i], 0x8e); /* 10001110b */
	}

	/* Fill IDTR. */
	idtr.base = (uint32_t) idt;
	idtr.limit = sizeof(idt) - 1;

	/* Load IDT. */
	asm("lidt (%0)" : : "p"(pidtr));
}

void set_idt_descriptor(idt_descriptor_t *desc, uint16_t selector, uint32_t offset, uint8_t flags)
{
	desc->offset_low = offset & 0xffff;
	desc->selector = selector;
	desc->fixed = 0;
	desc->flags = flags;
	desc->offset_high = (offset >> 16) & 0xffff;
}
