#include "geheugen.h"

void timer_interrupt_handler() {
    cpu_context_t *old = &current_task;
    cpu_context_t *new = &current_task;
    context_switch(old, new);
    acknowledge_interrupt();
    // After this, the new task is running.
}