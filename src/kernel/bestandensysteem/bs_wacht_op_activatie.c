#include "bestandensysteem.h"
#include "geheugen.h"

int filesystem_is_ready = 0;
void wachten_op_bestandssysteem()
{
    filesystem_is_ready = 0;
    agg:
    if(filesystem_is_ready==1)
    {
        return;
    }
    goto agg;
}