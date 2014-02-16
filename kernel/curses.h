#ifndef KERNEL_CURSES_H
#define KERNEL_CURSES_H

/* Colors. */
#define COLOR_BLACK               0
#define COLOR_BLUE                1
#define COLOR_GREEN               2
#define COLOR_CYAN                3
#define COLOR_RED                 4
#define COLOR_MAGENTA             5
#define COLOR_BROWN               6
#define COLOR_LIGHT_GRAY          7
#define COLOR_DARK_GRAY           8
#define COLOR_LIGHT_BLUE          9
#define COLOR_LIGHT_GREEN         10
#define COLOR_LIGHT_CYAN          11
#define COLOR_LIGHT_RED           12
#define COLOR_LIGHT_MAGENTA       13
#define COLOR_YELLOW              14
#define COLOR_WHITE               15

#define A_NORMAL                  0x07

#define A_FG_COLOR(x)             ((x) & 0x0f)
#define A_BG_COLOR(x)             (((x) & 0x0f) << 4)

#define MAKE_CHARACTER(c, fg, bg) (((c) & 0xff) | ((A_FG_COLOR(fg) | A_BG_COLOR(bg)) << 8))

#define FG_COLOR(ch)              (((ch) & 0x0f00) >> 8)
#define BG_COLOR(ch)              (((ch) & 0xf000) >> 12)

typedef unsigned short chtype;

void clear(void);
void curs_set(int visibility);
void attrset(int attrs);
void mvaddch(int y, int x, const chtype ch);
chtype mvinch(int y, int x);
void mvaddstr(int y, int x, const char *str);
void mvaddnstr(int y, int x, const char *str, int n);
void mvprintw(int y, int x, const char *fmt, ...);
void mvrect(int y, int x, int h, int w, chtype ch);
void mvhline(int y, int x, chtype ch, int n);
void mvvline(int y, int x, chtype ch, int n);

#endif /* KERNEL_CURSES_H */
