#include "bestandensysteem.h"
#include "geheugen.h"

void wachten_op_bestandssysteem()
{
    fshook = 0;
    agg:
    if(fshook!=0)
    {
        return;
    }
    sleep(100);
    goto agg;
}