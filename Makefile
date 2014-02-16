CC=gcc
AS=as
OBJCOPY=objcopy

CFLAGS=-m32 -Wall -Wno-long-long -pedantic -D_GNU_SOURCE -nostdinc -fno-stack-protector -fno-builtin -nostdlib -I.
ASFLAGS=--32

LDFLAGS=-Tlinker.ld
LIBS=

MAKEDEPEND=${CC} -MM

BOOTSECTOR=bootsect
KERNEL=nibblesOS
IMAGE=NibblesOS.iso

BOOTSECTOR_OBJS =	asm/16-bit/bootsect.o

KERNEL_OBJS =		kernel/kmain.o kernel/idt.o asm/32-bit/exception_handler.o asm/32-bit/irq_handler.o \
			asm/32-bit/interrupt_handler.o kernel/exception_handler.o kernel/irq_handler.o \
			kernel/interrupt_handler.o kernel/string.o kernel/curses.o kernel/system_timer.o \
			kernel/keyboard.o kernel/printf.o kernel/usleep.o kernel/random.o \
			kernel/nibbles/snake.o kernel/nibbles/inputs.o kernel/nibbles/field.o \
			kernel/nibbles/intro.o kernel/nibbles/nibbles.o

DEPS:= ${KERNEL_OBJS:%.o=%.d}

all: $(IMAGE)

${IMAGE}: ${BOOTSECTOR} ${KERNEL}
	dd if=${BOOTSECTOR} of=cd/image.img
	dd seek=1 if=${KERNEL} of=cd/image.img
	genisoimage -o $@ -A ${KERNEL} -b image.img cd

${BOOTSECTOR}: ${BOOTSECTOR_OBJS}
	${OBJCOPY} ${BOOTSECTOR_OBJS} -O binary $@

${KERNEL}: ${KERNEL_OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} ${KERNEL_OBJS} ${LIBS} -o $@
	${OBJCOPY} --pad-to=1505792 $@ -O binary $@

clean:
	rm -f ${BOOTSECTOR} ${KERNEL} ${BOOTSECTOR_OBJS} ${KERNEL_OBJS} ${DEPS} ${IMAGE} cd/image.img

${BOOTSECTOR_OBJS} ${KERNEL_OBJS} ${DEPS} ${KERNEL} : Makefile

.PHONY : all clean

%.d : %.c
	${MAKEDEPEND} ${CFLAGS} $< -MT ${@:%.d=%.o} > $@

%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $<

%.o : %.s
	${AS} ${ASFLAGS} -o $@ $<

-include ${DEPS}
