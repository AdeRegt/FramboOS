#include "geheugen.h"

void hlt()
{
    asm volatile ("hlt");
}