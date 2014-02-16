NibblesOS
=========

NibblesOS is a clone of the 'QBasic Nibbles' game that up to four players can play simultaneously.

NibblesOS (Nibbles + OS) has three main components:
* Boot sector.
* OS: 32-bit "operating system".
 The OS handles only two IRQs:
  * System timer (so that the system can sleep a certain time between moves of the snakes).
  * Keyboard.
* Nibbles: the game itself.


### Internals
When the computer boots, the boot sector (`asm/16-bit/bootsect.s`) is loaded into memory and executed. The boot sector reads the kernel (`asm/16-bit/read_kernel.s`, which just reads a bunch of sectors starting at sector 2), switches to protected mode and calls the kernel (`kernel/kmain.c`). `kmain` sets the IDT, initializes the PIC 8259 and starts the game.


### Compiling
Just execute `make` (`genisoimage` is required to generate the ISO image).


### Running
`make` generates an ISO image named `NibblesOS.iso`.

With VirtualBox or VMWare create a new virtual machine with an unknown operating system, no hard disk and a memory size of 4 MB. Set the `NibblesOS.iso` ISO file as a virtual CD disk file.



### Differences from the original game
* Up to four players can play simultaneously.
* Monochrome mode is not supported.
* Sound has not been implemented.


### Screenshots
Introduction screen:

![alt text](https://github.com/guidoreina/nibblesos/raw/master/screenshots/intro.png "Introduction screen")

Inputs screen:

![alt text](https://github.com/guidoreina/nibblesos/raw/master/screenshots/inputs.png "Introduction screen")

Playing:

![alt text](https://github.com/guidoreina/nibblesos/raw/master/screenshots/playing.png "Introduction screen")
