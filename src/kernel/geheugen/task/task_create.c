#include "geheugen.h"

void task_create(char* name, void (*func)()){
    while(1){
        if(ct[0].context.rip != 0){
            break;
        }
        sleep(100);
    }
    cli();
    memset(&ct[max_task], 0, sizeof(task_t));
    memcpy(&ct[max_task].context, &ct[0].context, sizeof(cpu_context_t));
    ct[max_task].context.rip = (uint64_t) func;
    ct[max_task].state = 1;
    memcpy(ct[max_task].name, name, 32);
    max_task++;
    #ifdef DEBUG_TASK
    printk("Taak aangemaakt: %s ( taak id: %d )\n", name, max_task - 1);
    #endif 
    // for(;;);
    sti();
}