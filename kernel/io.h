#ifndef KERNEL_IO_H
#define KERNEL_IO_H

static inline unsigned char inb(unsigned short port)
{
	unsigned char value;

	asm("inb %w1, %0": "=a"(value): "Nd"(port));
	return value;
}

static inline void outb(unsigned short port, unsigned char value)
{
	asm("outb %b0, %w1": : "a"(value), "Nd"(port));
}

#endif /* KERNEL_IO_H */
