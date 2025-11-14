#include "geheugen.h"

void timersleep(uint64_t secs)
{
    ct[current_task].timer = 0;
    while(ct[current_task].timer<secs){
        // wacht
    }
}