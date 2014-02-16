.intel_syntax noprefix                            # Enable Intel syntax.

####################################################################
####################################################################
##                                                                ##
## Function: read_kernel.                                         ##
## Description: Reads the kernel boot sectors from the floppy     ##
## disk. The kernel boot sectors starts at Cylinder 0, Head 0 and ##
## Sector 2.                                                      ##
## Parameters:                                                    ##
##     - (in) ES:BX => Points to the data buffer where the        ##
##       information should be stored.                            ##
##                                                                ##
####################################################################
####################################################################
read_kernel:
    mov di, 5                                     # Number of retries.

read_sectors:
    mov ah, 2
    mov al, 100                                   # Read 100 sectors (100 * 512 = 51200 bytes).
    mov cx, 2
    xor dh, dh
    movb dl, [drive]
    int 0x13                                      # Read sector(s) into memory.
    jnc read_ok

    ######################################
    ######################################
    ##                                  ##
    ## Error reading sector(s).         ##
    ##                                  ##
    ######################################
    ######################################
    xor ah, ah                                    # AH <-- 0.
    int 0x13                                      # Reset disk system.

    dec di                                        # Decrement number of retries.
    jz read_error                                 # If DI = 0 we won't retry it anymore.
    jmp read_sectors                              # DI is greater than 0, try to read again.

read_ok:
    ret

read_error:
    stc                                           # Set the carry flag.
    ret
