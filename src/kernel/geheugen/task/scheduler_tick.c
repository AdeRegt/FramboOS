#include "geheugen.h"

cpu_context_t* get_current_task_context() {
    printk("currenttask: %d maxtask: %d \n",current_task,max_task);
    return &ct[current_task].context;
}

void scheduler_tick() {
    // Markeer dat deze taak zijn tijdslot heeft gehad
    ct[current_task].timerswatch = 0;

    // Zoek de volgende actieve taak (round-robin vanaf current_task + 1)
    int start_task = current_task;
    int next_task = current_task;
    do {
        next_task = (next_task + 1 ) % max_task;
        if (ct[next_task].state == 1) {
            current_task = next_task;
            break;
        }
    } while (next_task != start_task);
    // if(current_task==0&&max_task==2){
    //     current_task = 1;
    // }else{
    //     current_task = 0;
    // }

    // Zorg dat de timerswatch voor de nieuwe taak klaarstaat
    ct[current_task].timerswatch = 1;

    acknowledge_interrupt();
}