#include "geheugen.h"

cpu_context_t* scheduler_tick(cpu_context_t* current) {
    ct[current_task].context = *current;

    current_task = (current_task + 1) % max_task;

    acknowledge_interrupt();

    *current = ct[current_task].context;
    return current;
}
