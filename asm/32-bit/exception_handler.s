.intel_syntax noprefix                            # Enable Intel syntax.

####################################################################
####################################################################
##                                                                ##
## Exception handlers.                                            ##
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

.macro exception_without_error_code number
    .globl exception_handler\number

    exception_handler\number:
        pusha                                     # Save general purpose registers.
        push dword ptr \number                    # Push exception number.

        call exception_handler

        add esp, 4                                # Remove exception number from the stack.
        popa                                      # Restore general purpose registers.

        iret
.endm

.macro exception_with_error_code number
    .globl exception_handler\number

    exception_handler\number:
        pusha                                     # Save general purpose registers.
        push dword ptr \number                    # Push exception number.

        call exception_handler

        add esp, 4                                # Remove exception number from the stack.
        popa                                      # Restore general purpose registers.

        # Remove error code from the stack.
        # According to the Intel's Software Developer's Manual:
        # Note that the error code is not popped when the IRET instruction is
        # executed to return from an exception handler, so the handler must
        # remove the error code before executing a return.
        add esp, 4

        iret
.endm

exception_without_error_code 0
exception_without_error_code 1
exception_without_error_code 2
exception_without_error_code 3
exception_without_error_code 4
exception_without_error_code 5
exception_without_error_code 6
exception_without_error_code 7
exception_with_error_code 8
exception_without_error_code 9
exception_with_error_code 10
exception_with_error_code 11
exception_with_error_code 12
exception_with_error_code 13
exception_with_error_code 14
exception_without_error_code 15
exception_without_error_code 16
exception_with_error_code 17
exception_without_error_code 18
exception_without_error_code 19
exception_without_error_code 20
exception_without_error_code 21
exception_without_error_code 22
exception_without_error_code 23
exception_without_error_code 24
exception_without_error_code 25
exception_without_error_code 26
exception_without_error_code 27
exception_without_error_code 28
exception_without_error_code 29
exception_without_error_code 30
exception_without_error_code 31
