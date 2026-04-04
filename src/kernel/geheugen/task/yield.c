#include "geheugen.h"

void yield()
{
    ct[current_task].timerswatch = 0;
    while(1)
    {
        if(ct[current_task].timerswatch != 0)
        {
            break;
        }
        sleep(100);
    }
}