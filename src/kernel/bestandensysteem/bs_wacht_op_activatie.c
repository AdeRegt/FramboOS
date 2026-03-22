#include "bestandensysteem.h"
#include "geheugen.h"

void wachten_op_bestandssysteem()
{
    while(1)
    {
        if(fshook!=0)
        {
            return;
        }
        sleep(100);
    }
}