#ifndef KERNEL_STDARG_H
#define KERNEL_STDARG_H

#define _INTSIZEOF(x) ((sizeof(x) + sizeof(int) - 1) & ~(sizeof(int) - 1))

typedef char *va_list;

#define va_start(ap, last) ((ap) = (va_list) &(last) + _INTSIZEOF(last))
#define va_arg(ap, type)   (*(type *) (((ap) += _INTSIZEOF(type)) - _INTSIZEOF(type)))
#define va_end(ap)         ((ap) = (va_list) 0)

#endif /* KERNEL_STDARG_H */
