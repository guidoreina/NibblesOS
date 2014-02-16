# guidOS's boot sector.

.intel_syntax noprefix                            # Enable Intel syntax.

.globl start
start:
    .code16                                       # Generate 16-bit code.

    .equ NEW_CS, 0x0000
    .equ NEW_IP, 0x0600                           # After relocation CS:IP = 0000:0600.

    jmp bootsector_main                           # Skip functions.

.include "asm/16-bit/display_string.s"
.include "asm/16-bit/read_kernel.s"
.include "asm/16-bit/enable_a20_gate.s"
.include "asm/16-bit/remap_pic.s"
.include "asm/32-bit/gdt.s"


####################################################################
####################################################################
##                                                                ##
## Main function.                                                 ##
##                                                                ##
####################################################################
####################################################################
bootsector_main:
    ######################################
    ######################################
    ##                                  ##
    ## Initialize stack pointer (SS:SP) ##
    ## to physical address 0000:7c00.   ##
    ##                                  ##
    ######################################
    ######################################
    cli                                           # Disable interrupts.
    xor ax, ax                                    # AX <-- 0x0000.
    mov ss, ax                                    # SS <-- 0x0000.
    mov sp, 0x7c00                                # SS:SP <-- 0000:7c00.
    sti                                           # Enable interrupts.

    ######################################
    ######################################
    ##                                  ##
    ## Relocate boot sector to physical ##
    ## address 0000:0600.               ##
    ##                                  ##
    ######################################
    ######################################
    mov ds, ax                                    # DS <-- 0x0000.
    mov si, 0x7c00                                # DS:SI <-- 0000:7c00.

    mov ax, NEW_CS
    mov es, ax                                    # ES <-- NEW_CS.
    mov di, NEW_IP                                # ES:DI <-- NEW_CS:NEW_IP.

    cld                                           # Clear direction flag.

    mov cx, 512 / 2                               # CX <-- 256 (words).

    rep movsw                                     # Relocate boot sector to physical address NEW_CS:NEW_IP (0000:0600).

    jmp NEW_CS:NEW_IP + offset relocated          # Jump to relocated code.

relocated:
    ######################################
    ######################################
    ##                                  ##
    ## Initialize data segment (DS) to  ##
    ## 0x0060.                          ##
    ##                                  ##
    ######################################
    ######################################
    # The boot sector has been relocated to NEW_CS:NEW_IP.
    # As .org = 0x0, DS has to be setup to NEW_CS + (NEW_IP / 0x10).
    mov ax, NEW_CS + (NEW_IP / 0x10)              # AX <-- 0x0060.
    mov ds, ax                                    # DS <-- 0x0060.

    ######################################
    ######################################
    ##                                  ##
    ## Save the drive we booted from    ##
    ## (the BIOS sets DL to the boot    ##
    ## drive).                          ##
    ##                                  ##
    ######################################
    ######################################
    movb [drive], dl

    ######################################
    ######################################
    ##                                  ##
    ## Display loading message.         ##
    ##                                  ##
    ######################################
    ######################################
    lea si, [loading_message]
    call display_string

    ######################################
    ######################################
    ##                                  ##
    ## Read kernel boot sectors into    ##
    ## physical address 0000:7c00.      ##
    ##                                  ##
    ######################################
    ######################################
    xor ax, ax                                    # AX <-- 0x0000.
    mov es, ax                                    # ES <-- 0x0000.
    mov bx, 0x7c00                                # BX <-- 0x7c00
    call read_kernel
    jc error_reading_kernel

    ######################################
    ######################################
    ##                                  ##
    ## Enable A20 Gate.                 ##
    ##                                  ##
    ######################################
    ######################################
    call enable_a20_gate

    ######################################
    ######################################
    ##                                  ##
    ## Remap PIC.                       ##
    ##                                  ##
    ######################################
    ######################################
    call remap_pic

    ######################################
    ######################################
    ##                                  ##
    ## Switch to protected mode.        ##
    ##                                  ##
    ######################################
    ######################################
    lea si, [entering_protected_mode_message]
    call display_string

    cli                                           # Disable interrupts.

    lgdt [GDTR]                                   # Load [GDTR] into the Global Descriptor Table Register.

    mov eax, cr0                                  # EAX <-- CR0.
    or al, 1                                      # Set PE bit (bit #0) in AL.
    mov cr0, eax                                  # Switch to protected mode.

    # Clear prefetch queue.
    jmp 8:(NEW_CS * 0x10) + NEW_IP + offset protected_mode

protected_mode:
    .code32

    ######################################
    ######################################
    ##                                  ##
    ## Initialize data segment          ##
    ## registers to 10h (entry #2 in    ##
    ## the GDT).                        ##
    ##                                  ##
    ######################################
    ######################################
    mov eax, 0x10                                 # AX <-- 16 (Offset of data segment descriptor in the GDT).
    mov ds, eax
    mov es, eax
    mov ss, eax
    mov fs, eax
    mov gs, eax
    mov esp, 0x7c00

    call 8:0x7c00

    hlt                                           # Halt the system.

error_reading_kernel:
    .code16                                       # Generate 16-bit code.

    ######################################
    ######################################
    ##                                  ##
    ## Kernel boot sectors couldn't be  ##
    ## read.                            ##
    ##                                  ##
    ######################################
    ######################################
    lea si, [io_error_message]
    call display_string

    ######################################
    ######################################
    ##                                  ##
    ## Halt system.                     ##
    ##                                  ##
    ######################################
    ######################################
end:
    hlt


####################################################################
####################################################################
##                                                                ##
## Variables.                                                     ##
##                                                                ##
####################################################################
####################################################################
loading_message:                 .asciz "Loading guidOS...\r\n"
drive:                           .byte  0x00                  # Drive we booted from.
io_error_message:                .asciz "I/O error.\r\n"
entering_protected_mode_message: .asciz "Entering protected mode...\r\n"


.fill 0x1fe - (. - start), 1, 0                   # Set the rest of the sector to 0's.
.word 0xaa55                                      # A boot sector must end with the two bytes: aa 55.
