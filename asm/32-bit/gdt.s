.intel_syntax noprefix                            # Enable Intel syntax.

####################################################################
####################################################################
##                                                                ##
## Global Descriptor Table (GDT).                                 ##
##                                                                ##
####################################################################
####################################################################

# The base address of the GDT should be aligned on an eight-byte
# boundary to yield the best processor performance.
.align 8

GDT:
    ######################################
    ######################################
    ##                                  ##
    ## First entry: NULL descriptor.    ##
    ##                                  ##
    ######################################
    ######################################
    .quad 0


    ######################################
    ######################################
    ##                                  ##
    ## Second entry: Code segment       ##
    ## descriptor.                      ##
    ##                                  ##
    ######################################
    ######################################
    .word 0xffff                                  # Segment Limit 15..00 = 4 GB.
    .word 0x0000                                  # Base Address 15..00 = 0x0000.
    .byte 0x00                                    # Base 23..16 = 0x00.
    .byte 0b10011010                              # From left to right:
                                                  #   P = 1. Segment Present flag: Indicates whether the segment is present in memory (set) or
                                                  #          not present (clear).
                                                  #   DPL = 00. Descriptor Privilege Level: Specifies the privilege level of the segment.
                                                  #   S = 1. Descriptor type flag: Specifies whether the segment descriptor is for a
                                                  #          system segment (S flag is clear) or a code or data segment (S flag is set).
                                                  #   Type = 1010. Execute / Read segment.
    .byte 0b11001111                              # From left to right:
                                                  #   G = 1. Granularity flag: Determines the scaling of the segment limit field. When
                                                  #          the granularity flag is clear, the segment limit is interpreted in byte units;
                                                  #          when flag is set, the segment limit is interpreted in 4-KByte units.
                                                  #   D/B = 1. Default operation size/default stack pointer size and/or upper bound flag.
                                                  #            This flag should always be set to 1 for 32-bit code and data segments and to 0
                                                  #            for 16-bit code and data segments.
                                                  #   L = 0. 64-bit code segment flag. When not in IA-32e mode or for non-code segments,
                                                  #          bit 21 is reserved and should always be set to 0.
                                                  #   AVL = 0. Available for use by system software.
                                                  #   Segment Limit 19..16 = 0xf.
    .byte 0x00                                    # Base 31..24 = 0x00.


    ######################################
    ######################################
    ##                                  ##
    ## Third entry: Data segment        ##
    ## descriptor.                      ##
    ##                                  ##
    ######################################
    ######################################
    .word 0xffff                                  # Segment Limit 15..00 = 4 GB.
    .word 0x0000                                  # Base Address 15..00 = 0x0000.
    .byte 0x00                                    # Base 23..16 = 0x00.
    .byte 0b10010010                              # From left to right:
                                                  #   P = 1. Segment present: Indicates whether the segment is present in memory (set) or
                                                  #          not present (clear).
                                                  #   DPL = 00. Descriptor Privilege Level: Specifies the privilege level of the segment.
                                                  #   S = 1. Descriptor type flag: Specifies whether the segment descriptor is for a
                                                  #          system segment (S flag is clear) or a code or data segment (S flag is set).
                                                  #   Type = 0010. Read / Write segment.
    .byte 0b11001111                              # From left to right:
                                                  #   G = 1. Granularity flag: Determines the scaling of the segment limit field. When
                                                  #          the granularity flag is clear, the segment limit is interpreted in byte units;
                                                  #          when flag is set, the segment limit is interpreted in 4-KByte units.
                                                  #   D/B = 1. Default operation size/default stack pointer size and/or upper bound flag.
                                                  #            This flag should always be set to 1 for 32-bit code and data segments and to 0
                                                  #            for 16-bit code and data segments.
                                                  #   L = 0. 64-bit code segment flag. When not in IA-32e mode or for non-code segments,
                                                  #          bit 21 is reserved and should always be set to 0.
                                                  #   AVL = 0. Available for use by system software.
                                                  #   Segment Limit 19..16 = 0xf.
    .byte 0x00                                    # Base 31..24 = 0x00.


.equ GDT_SIZE, (. - GDT)


####################################################################
####################################################################
##                                                                ##
## Global Descriptor Table Register (GDTR).                       ##
##                                                                ##
####################################################################
####################################################################

GDTR:
    .word GDT_SIZE - 1                            # 16-bit limit.
    .long NEW_IP + offset GDT                     # 32-bit base address.
