.intel_syntax noprefix                            # Enable Intel syntax.

####################################################################
####################################################################
##                                                                ##
## Function: display_string.                                      ##
## Description: Displays a string through the BIOS services.      ##
## Parameters:                                                    ##
##     - (in) DS:SI => Points to the string to be displayed.      ##
##                                                                ##
####################################################################
####################################################################
display_string:
    lodsb                                         # Load byte at address DS:SI into AL.
    or al, al                                     # Is it '\0'?
    jz end_display                                # If it is, we are done.

    mov ah, 0x0e
    mov bx, 0007
    int 0x10                                      # Teletype output.

    jmp display_string                            # Display next character.

end_display:
    ret
