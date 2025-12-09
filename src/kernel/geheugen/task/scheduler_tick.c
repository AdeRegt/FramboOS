#include "geheugen.h"

cpu_context_t* scheduler_tick(cpu_context_t* current){
    #ifdef DEBUG_TASK
    printk("Scheduler tick: wisselen van taak %d met naam %s ", current_task,ct[current_task].name);
    #endif 
    memcpy(&ct[current_task].context, current, sizeof(cpu_context_t));
    current_task++;
    if(current_task >= max_task){
        current_task = 0;
    }
    for(int i = 0; i < max_task; i++){
        ct[i].timer++;
    }
    acknowledge_interrupt();
    #ifdef DEBUG_TASK
    printk("Naar taak %d met naam %s \n", current_task,ct[current_task].name);
    #endif 
    memcpy(current, &ct[current_task].context, sizeof(cpu_context_t));    
    return current;
}
