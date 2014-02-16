.intel_syntax noprefix                            # Enable Intel syntax.

####################################################################
####################################################################
##                                                                ##
## Interrupt handlers.                                            ##
##                                                                ##
####################################################################
####################################################################

.align 4

# The interrupts have been already disabled by the processor.
# The IDT descriptors are interrupt gates and, according to the Intel's
# Software Developer's Manual:
# When accessing an exception- or interrupt-handling procedure through
# an interrupt gate, the processor clears the IF flag to prevent other
# interrupts from interfering with the current interrupt handler. A
# subsequent IRET instruction restores the IF flag to its value in the
# saved contents of the EFLAGS register on the stack.

.macro interrupt number
    .globl interrupt_handler\number

    interrupt_handler\number:
        pusha                                     # Save general purpose registers.
        push dword ptr \number                    # Push interrupt number.

        call interrupt_handler

        add esp, 4                                # Remove interrupt number from the stack.
        popa                                      # Restore general purpose registers.

        iret
.endm

interrupt 0
interrupt 1
interrupt 2
interrupt 3
interrupt 4
interrupt 5
interrupt 6
interrupt 7
interrupt 8
interrupt 9
interrupt 10
interrupt 11
interrupt 12
interrupt 13
interrupt 14
interrupt 15
interrupt 16
interrupt 17
interrupt 18
interrupt 19
interrupt 20
interrupt 21
interrupt 22
interrupt 23
interrupt 24
interrupt 25
interrupt 26
interrupt 27
interrupt 28
interrupt 29
interrupt 30
interrupt 31
