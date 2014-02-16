#ifndef KERNEL_IRQ_HANDLER_H
#define KERNEL_IRQ_HANDLER_H

typedef void (*irq_handler_t)(void);

void irq_handler0(void);
void irq_handler1(void);
void irq_handler2(void);
void irq_handler3(void);
void irq_handler4(void);
void irq_handler5(void);
void irq_handler6(void);
void irq_handler7(void);
void irq_handler8(void);
void irq_handler9(void);
void irq_handler10(void);
void irq_handler11(void);
void irq_handler12(void);
void irq_handler13(void);
void irq_handler14(void);
void irq_handler15(void);

void irq_handler(unsigned number);

#endif /* KERNEL_IRQ_HANDLER_H */
