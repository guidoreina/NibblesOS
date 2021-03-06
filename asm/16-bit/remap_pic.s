.intel_syntax noprefix                            # Enable Intel syntax.

####################################################################
####################################################################
##                                                                ##
## Function: remap_pic.                                           ##
## Description: Remaps interrupts (in protected mode the first 32 ##
## interrupts are reserved by Intel).                             ##
##                                                                ##
####################################################################
####################################################################
remap_pic:
    cli                                           # Disable interrupts.

    ######################################
    ######################################
    ##                                  ##
    ## Send ICW1 to both PICs.          ##
    ##                                  ##
    ######################################
    ######################################
    mov al, 0x11                                  # ICW4 needed, Cascade Mode (more than one PIC).
    out 0x20, al
    out 0xa0, al

    ######################################
    ######################################
    ##                                  ##
    ## Send ICW2 to both PICs.          ##
    ##                                  ##
    ######################################
    ######################################
    mov al, 0x20                                  # IRQ #0 starts at INT #32.
    out 0x21, al
    mov al, 0x28                                  # IRQ #8 starts at INT #40 (second PIC).
    out 0xa1, al

    ######################################
    ######################################
    ##                                  ##
    ## Send ICW3 to both PICs.          ##
    ##                                  ##
    ######################################
    ######################################
    mov al, 4                                     # IR2 is connected to a slave (INT #2).
    out 0x21, al
    mov al, 2                                     # Slave ID (master IR input).
    out 0xa1, al

    ######################################
    ######################################
    ##                                  ##
    ## Send ICW4 to both PICs.          ##
    ##                                  ##
    ######################################
    ######################################
    mov al, 1                                     # 8086 mode.
    out 0x21, al
    out 0xa1, al

    sti                                           # Enable interrupts.

    ret
