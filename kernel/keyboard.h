#ifndef KERNEL_KEYBOARD_H
#define KERNEL_KEYBOARD_H

/* Scan codes. */
#define SCANCODE_UP           (0x80 | 0x48)
#define SCANCODE_DOWN         (0x80 | 0x50)
#define SCANCODE_LEFT         (0x80 | 0x4b)
#define SCANCODE_RIGHT        (0x80 | 0x4d)
#define SCANCODE_KEYPAD_UP    0x48
#define SCANCODE_KEYPAD_DOWN  0x50
#define SCANCODE_KEYPAD_LEFT  0x4b
#define SCANCODE_KEYPAD_RIGHT 0x4d
#define SCANCODE_W            0x11
#define SCANCODE_S            0x1f
#define SCANCODE_A            0x1e
#define SCANCODE_D            0x20
#define SCANCODE_O            0x18
#define SCANCODE_L            0x26
#define SCANCODE_K            0x25
#define SCANCODE_SEMICOLON    0x27
#define SCANCODE_SPACE        0x39
#define SCANCODE_Y            0x15
#define SCANCODE_N            0x31
#define SCANCODE_1            0x02
#define SCANCODE_2            0x03
#define SCANCODE_3            0x04
#define SCANCODE_4            0x05
#define SCANCODE_5            0x06
#define SCANCODE_6            0x07
#define SCANCODE_7            0x08
#define SCANCODE_8            0x09
#define SCANCODE_9            0x0a
#define SCANCODE_0            0x0b
#define SCANCODE_P            0x19
#define SCANCODE_BACKSPACE    0x0e
#define SCANCODE_ENTER        0x1c

void keyboard_init(void);
void keyboard_irq(void);
int keyboard_get_key(void);
int keyboard_wait_for_key(void);

#endif /* KERNEL_KEYBOARD_H */
