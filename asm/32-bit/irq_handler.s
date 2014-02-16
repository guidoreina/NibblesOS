.intel_syntax noprefix                            # Enable Intel syntax.

####################################################################
####################################################################
##                                                                ##
## IRQ handlers.                                                  ##
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

.macro irq number
    .globl irq_handler\number

    irq_handler\number:
        pusha                                     # Save general purpose registers.
        push dword ptr \number                    # Push IRQ number.

        call irq_handler

        add esp, 4                                # Remove IRQ number from the stack.
        popa                                      # Restore general purpose registers.

        iret
.endm

irq 0
irq 1
irq 2
irq 3
irq 4
irq 5
irq 6
irq 7
irq 8
irq 9
irq 10
irq 11
irq 12
irq 13
irq 14
irq 15
