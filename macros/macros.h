#ifndef MACROS_H
#define MACROS_H

#define IS_ALPHA(x)   ((((x) >= 'A') && ((x) <= 'Z')) || (((x) >= 'a') && ((x) <= 'z')))
#define IS_DIGIT(x)   (((x) >= '0') && ((x) <= '9'))

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))

#endif /* MACROS_H */
