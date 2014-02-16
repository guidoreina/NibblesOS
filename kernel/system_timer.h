#ifndef KERNEL_SYSTEM_TIMER_H
#define KERNEL_SYSTEM_TIMER_H

void system_timer_init(void);
void system_timer_irq(void);
void system_timer_reset_ticks(void);
unsigned system_timer_ticks(void);

#endif /* KERNEL_SYSTEM_TIMER_H */
