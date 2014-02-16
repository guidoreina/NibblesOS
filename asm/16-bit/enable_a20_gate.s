.intel_syntax noprefix                            # Enable Intel syntax.

####################################################################
####################################################################
##                                                                ##
## Function: enable_a20_gate.                                     ##
## Description: Enables the A20 gate, which enables the CPU to    ##
## access all the memory.                                         ##
##                                                                ##
####################################################################
####################################################################
enable_a20_gate:
    cli                                           # Disable interrupts.

    call wait_kbd_input_buffer_empty

    mov al, 0xad
    out 0x64, al                                  # Command: Disable keyboard clock.

    call wait_kbd_input_buffer_empty

    mov al, 0xd0
    out 0x64, al                                  # Command: Read output port.

    call wait_kbd_output_buffer_not_empty         # Wait for the data to be available.

    in al, 0x60                                   # Read data from output port.

    push eax                                      # Save data.

    call wait_kbd_input_buffer_empty

    mov al, 0xd1
    out 0x64, al                                  # Command: Write output port.

    call wait_kbd_input_buffer_empty

    pop eax                                       # Restore data.
    or al, 2                                      # Set bit 1 (A20).
    out 0x60, al                                  # Enable A20 gate.

    call wait_kbd_input_buffer_empty

    mov al, 0xae
    out 0x64, al                                  # Command: Enable keyboard clock.

    call wait_kbd_input_buffer_empty

    sti                                           # Enable interrupts.

    ret


####################################################################
####################################################################
##                                                                ##
## Function: wait_kbd_input_buffer_empty.                         ##
## Description: Waits for the keyboard input buffer to be empty.  ##
##                                                                ##
####################################################################
####################################################################
wait_kbd_input_buffer_empty:
    in al, 0x64                                   # Read status register into AL.
    test al, 2                                    # If bit 1 is set, the input buffer is full.
    jnz wait_kbd_input_buffer_empty
    ret


####################################################################
####################################################################
##                                                                ##
## Function: wait_kbd_output_buffer_not_empty.                    ##
## Description: Waits for the keyboard output buffer to have      ##
## data.                                                          ##
##                                                                ##
####################################################################
####################################################################
wait_kbd_output_buffer_not_empty:
    in al, 0x64                                   # Read status register into AL.
    test al, 1                                    # If bit 0 is not set, the output buffer is empty.
    jz wait_kbd_output_buffer_not_empty
    ret
